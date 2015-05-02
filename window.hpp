#ifndef TWOMBLY_WINDOW_HEADER
#define TWOMBLY_WINDOW_HEADER

#include <glob.h>
#include <sys/mman.h>

#include "twombly.hpp"
#include <SDL2/SDL.h>

namespace tw {

using std::stringstream;
using std::string;

string windowTitle(const char *title, int w, int h, int c);
bool parseWindowTitle(const char *path, string *name, int32_t *w, int32_t *h, int16_t *c);
void *diskmem(const char *path, size_t sz);

template<typename DiskImageType>
DiskImageType loadFromDisk(const char *path){
    string name;
    int32_t w, h;
    int16_t c;
    glob_t buf;
    glob(path, GLOB_TILDE, nullptr, &buf);
    DiskImageType dst;

    if (!parseWindowTitle(buf.gl_pathc ? buf.gl_pathv[0] : path, &name, &w, &h, &c)){
        globfree(&buf);
        return dst;
    }

    if (DiskImageType::_channels() != c){
        globfree(&buf);
        return dst;
    }

    uint8_t *data = (uint8_t*)diskmem(buf.gl_pathc ? buf.gl_pathv[0] : path, w * h * c);
    if (!data){
        globfree(&buf);
        return dst;
    }

    globfree(&buf);

    dst.width = w;
    dst.height = h;
    dst.alloc();
    for(auto i = 0; i < w * h; i++){
        for(auto j = 0; j < c; j++){
            dst[i][j] = data[i * c + j];
        }
    }

    munmap(data, w * h * c);

    return dst;
}

template<typename DiskImageType>
bool saveToDisk(DiskImageType &im, const char *path){
    string name;
    name = windowTitle(path, im.width, im.height, im.channels);

    return saveToDiskDirect(im, name.c_str());
}

template<typename DiskImageType>
bool saveToDiskDirect(DiskImageType &im, const char *path){
    uint8_t *data = (uint8_t*)diskmem(path, im.width * im.height * im.channels);
    if (!data){
        return false;
    }

    for(auto i = 0; i < im.width * im.height; i++){
        for(auto j = 0; j < im.channels; j++){
            data[i * im.channels + j] = im[i][j];
        }
    }

    munmap(data, im.width * im.height * im.channels);

    return true;
}

class Window {
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *surface;
    SDL_Event event;
    uint8_t *data;
    int32_t width, height;
    int16_t channels;

    Window(const char *title, int w, int h, int c = 4, int x = 0, int y = 0, int flags = SDL_WINDOW_SHOWN);
    Window(const char *title, int flags = SDL_WINDOW_SHOWN);
    ~Window();

    void show();

    template <typename ImportImageType>
    void import(ImportImageType &im){
        for(auto i = 0; i < width * height * channels; i+=channels){
            for(auto j = 0; j < channels; j++){
                data[i+j] = im[i/channels][j];
            }
        }
    }

    template <typename ExportImageType>
    void image(ExportImageType &d){
        if (!d.valid()){
            d.width = width;
            d.height = height;
            d.alloc();
        }

        if (d.channels != channels){
            throw std::runtime_error("channels don't match");
        }

        for(auto i = 0; i < width * height; i++){
            for(auto j = 0; j < channels; j++){
                d[i][j] = data[(i*channels) + j];
            }

        }
    }


    void import(uint8_t *d){
        memcpy(data, d, width * height * channels);
    }

    void image(uint8_t *d){
        memcpy(d, data, width * height * channels);
    }

    void update();
    bool poll(int t = 0);
    void setTexture();
};

} // namespace window

#endif // TWOMBLY_WINDOW_HEADER
