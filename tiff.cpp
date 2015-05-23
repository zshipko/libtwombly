#include "io.hpp"
#include <cstring>
#include <cmath>

#include <iostream>

namespace tw {

class AutoTIFF {
public:
    TIFF *handle;

    AutoTIFF(const char *path, const char *mode = "r") : handle(nullptr) {
        handle = tiffOpen(path, mode);
    }

    ~AutoTIFF(){
        tiffClose(handle);
    }

    bool ok(){
        return handle != nullptr;
    }
};

TIFF *tiffOpen(const char *path, const char *mode){
    return TIFFOpen(path, mode);
}

void tiffClose(TIFF *tif){
    if (tif)
        TIFFClose(tif);
}


bool writeTIFF(TIFF *tif, Image<uint8_t> const &im){
    if (!tif){
        return false;
    }

    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, im.height);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, im.width);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)im.channels);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);

    unsigned char scanline[TIFFScanlineSize(tif)];
    for(auto row = 0; row < im.height; row++){
        memcpy(scanline, im.data +  TIFFScanlineSize(tif) * row, TIFFScanlineSize(tif));
        TIFFWriteScanline(tif, scanline, row, 0);
    }

    return true;
}

bool writeTIFF(const char *path, Image<uint8_t> const &im){
    AutoTIFF tif(path, "w");

    if (!tif.ok())
        return false;

    return writeTIFF(tif.handle, im);
}

bool writeTIFF(TIFF *tif, Image<uint16_t> const &im){
    if (!tif){
        return false;
    }

    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, im.height);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, im.width);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)im.channels);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);

    unsigned char scanline[TIFFScanlineSize(tif)];
    for(auto row = 0; row < im.height; row++){
        memcpy(scanline, im.data +  TIFFScanlineSize(tif) * row, TIFFScanlineSize(tif));
        TIFFWriteScanline(tif, scanline, row, 0);
    }

    return true;
}

bool writeTIFF(const char *path, Image<uint16_t> const &im){
    AutoTIFF tif(path, "w");

    if (!tif.ok())
        return false;

    return writeTIFF(tif.handle, im);
}

Image<uint8_t> readTIFF(TIFF *tif){
    uint32_t width, height, row;
    uint16_t chan, depth;

    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &chan);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &depth);

    Image<uint8_t> im(width, height, chan);

    if (depth == 8){
        for (row = 0; row < height; row++){
            TIFFReadScanline(tif, im.data +  TIFFScanlineSize(tif) * row, row, 0);
        }
    } else if (depth == 16){
        Image<uint16_t> tmp(width, height, chan);
        for(row = 0; row < height; row++){
            TIFFReadScanline(tif, tmp.data + tmp.rowstride() * row, row, 0);
        }

        tmp.convertTo(im, [](uint16_t num){ return num >> 8; });
    }

    return im;
}

Image<uint8_t> readTIFF(const char *path){
    AutoTIFF tif(path);
    return readTIFF(tif.handle);
}

Image<uint16_t> readTIFF16(TIFF *tif){
    uint32_t width, height, row;
    uint16_t chan, depth;

    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &chan);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &depth);

    Image<uint16_t> im(width, height, chan);

    if (depth == 16){
        for (row = 0; row < height; row++){
            TIFFReadScanline(tif, im.data +  TIFFScanlineSize(tif) * row, row, 0);
        }
    } else if (depth == 8){
        Image<uint8_t> tmp(width, height, chan);
        for(row = 0; row < height; row++){
            TIFFReadScanline(tif, tmp.data + tmp.rowstride() * row, row, 0);
        }

        tmp.convertTo(im, [](uint8_t num){ return num << 8; });
    }

    return im;
}

Image<uint16_t> readTIFF16(const char *path){
    AutoTIFF tif(path);
    return readTIFF16(tif.handle);
}

} // namespace tw
