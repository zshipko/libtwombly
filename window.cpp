#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include "window.hpp"

static bool inited = false;

namespace tw {

class InitSDLClass {
public:
    InitSDLClass(){
        if (!inited){
            SDL_Init(SDL_INIT_EVERYTHING);
            atexit(SDL_Quit);
            inited = true;
        }
    }
};

static void InitSDL(){
    if (!inited){
        SDL_Init(SDL_INIT_EVERYTHING);
        atexit(SDL_Quit);
        inited = true;
    }
}

void *diskmem(const char *path, size_t sz){
    int fd = open(path, O_CREAT|O_RDWR, 0666);
    if (fd < 0){
        return nullptr;
    }

    lseek(fd, sz+1, SEEK_SET);
    write(fd, "", 1);

    void *data = mmap(nullptr, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    close(fd);
    return data;
}

string windowTitle(const char *title, int w, int h, int c){
    stringstream pth;
    pth << "tw-" << title << "-" << w << "-" << h << "-" << c;
    return pth.str();
}

bool parseWindowTitle(const char *path, string *name, int32_t *w, int32_t *h, int16_t *c){
    stringstream s(path);
    string tok;
    int32_t i32;
    int16_t i16;

    for(int i = 0; i < 5; i++){
        if (!std::getline(s, tok, '-')){
            return false;
        }

        stringstream t(tok);
        switch(i){
        default:
            continue;
        case 1:
            *name = tok;
            break;
        case 2:
            t >> i32;
            *w = i32;
            break;
        case 3:
            t >> i32;
            *h = i32;
            break;
        case 4:
            t >> i16;
            *c = i16;
            break;
        }
    }

    return true;
}

Window::Window(const char *title, int w, int h, int c, int x, int y, int flags)
         : channels(c), width(w), height(h){
    InitSDL();
    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    string path = windowTitle(title, w, h, c);
    data = (uint8_t*)diskmem(path.c_str(), w * h * c * sizeof(uint8_t));
    if (data){
        setTexture();
    }
}

Window::Window(const char *path, int flags){
    InitSDL();
    glob_t buf;
    glob(path, GLOB_TILDE, nullptr, &buf);
    string name;
    parseWindowTitle(buf.gl_pathc > 0 ? buf.gl_pathv[0] : path, &name, &width, &height, &channels);

    window = SDL_CreateWindow(name.c_str(), 0, 0, width, height, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    data = (uint8_t*)diskmem(buf.gl_pathc > 0 ? buf.gl_pathv[0] : path, width * height * channels);
    globfree(&buf);
    if (data){
        setTexture();
    }
}

Window::~Window(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (data){
        munmap(data, width * height * channels);
    }
}

void Window::setTexture(){
    if (channels == 4){
        surface = SDL_CreateRGBSurfaceFrom((uint8_t*)data, width, height, 32, width * channels,0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    } else if (channels == 3){
        surface = SDL_CreateRGBSurfaceFrom((uint8_t*)data, width, height, 24, width * channels, 0x0000FF, 0x00FF00, 0xFF0000, 0);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Window::show(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Window::update(){
    setTexture();
    show();
}

bool Window::poll(int t){
    SDL_WaitEventTimeout(&event, t);
    return event.type != SDL_QUIT;
}

} // namespace tw
