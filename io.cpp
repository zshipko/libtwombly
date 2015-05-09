#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "io.hpp"

#include <iostream>

namespace tw {

Image<uint8_t> imread(const char *path){
    int x,y,n;
    uint8_t *data = stbi_load(path, &x, &y, &n, 0);
    Image<uint8_t> im;
    if (!data){
#ifndef NO_TIFF
        uint16_t channels = 0;
        TIFF *t = tiffOpen(path, &channels, nullptr);
        if (!t){
            throw std::runtime_error("cannot load image");
        }
        im = openTIFF8(t);
#else // NO_TIFF
        throw std::runtime_error("cannot load image");
#endif // NO_TIFF

    } else {
        im = Image<uint8_t>(x, y, n, data);
    }
    return im;
}

bool imwrite(const char *p, Image<uint8_t> &im){
    std::string pth(p);
    std::string ext = pth.substr(pth.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    if (ext == "png") {
        return stbi_write_png(p, im.width, im.height, im.channels, (uint8_t*)im.data, im.width * im.channels) == 0;
#ifndef NO_TIFF
    } else if (ext == "tiff" || ext == "tif"){
        return saveTIFF(im, p);
#endif // NO_TIFF
    } else if (ext == "tga") {
        return stbi_write_tga(p, im.width, im.height, im.channels, (uint8_t*)im.data) == 0;
    } else if (ext == "bmp") {
        return stbi_write_bmp(p, im.width, im.height, im.channels, (uint8_t*)im.data) == 0;
    }
    return false;
}

#ifndef NO_TIFF
bool imwrite(const char *p, Image<uint16_t> &im){
    std::string pth(p);
    std::string ext = pth.substr(pth.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    if (ext == "tiff" || ext == "tif"){
        return saveTIFF(im, p);
    }
    return false;
}
#endif // NO_TIFF

} // namespace tw


