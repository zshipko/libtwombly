#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "io.hpp"

namespace tw {

bool imread(const char *path, Image<uint8_t> &im){
    int x,y,n;
    uint8_t *data = stbi_load(path, &x, &y, &n, 0);
    if (!data){
        throw std::runtime_error("cannot load image");
    }
    im = Image<uint8_t>(x, y, n, data);
    return im.valid();
}

bool imwrite(Image<uint8_t> &im, const char *p){
    std::string pth(p);
    std::string ext = pth.substr(pth.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    if (ext == "tiff" || ext == "tif"){
        return saveTIFF(im, p);
    } else if (ext == "png") {
        return stbi_write_png(p, im.width, im.height, im.channels, (uint8_t*)im.data, im.width * im.channels) == 0;
    } else if (ext == "tga") {
        return stbi_write_tga(p, im.width, im.height, im.channels, (uint8_t*)im.data) == 0;
    } else if (ext == "bmp") {
        return stbi_write_bmp(p, im.width, im.height, im.channels, (uint8_t*)im.data) == 0;
    }
    return false;
}

bool imwrite(Image<uint16_t> &im, const char *p){
    std::string pth(p);
    std::string ext = pth.substr(pth.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    if (ext == "tiff" || ext == "tif"){
        return saveTIFF(im, p);
    }
    return false;
}

} // namespace tw


