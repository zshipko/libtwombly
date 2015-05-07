#include "io.hpp"
#include <cstring>
#include <cmath>

#include <iostream>

namespace tw {

bool saveTIFF(Image<uint8_t> &im, const char *path, const char *mode){
    TIFF *tif = TIFFOpen(path, mode);
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

    TIFFClose(tif);

    return true;
}

bool saveTIFF(Image<uint16_t> &im, const char *path, const char *mode){
    TIFF *tif = TIFFOpen(path, mode);
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

    TIFFClose(tif);

    return true;
}

TIFF *tiffOpen(const char *path, uint16_t *channels, uint32_t *depth){
    TIFF *tif = TIFFOpen(path, "r");
    if (channels != nullptr && tif){
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &channels);
    }

    if (depth != nullptr && tif){
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &depth);

    }
    return tif;
}

Image<uint8_t> openTIFF8(TIFF *tif, double gamma){
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
            TIFFReadScanline(tif, tmp.data + TIFFScanlineSize(tif) * row, row, 0);
        }

        tmp.convertTo(im, [](uint16_t num){ return num >> 8; });
    }

    if (gamma > 1.0){
        for(size_t i = 0; i < im.length(); i++){
            im.data[i] = 255 * pow(im.data[i] / 255.0, 1/gamma);
        }
    }

    return im;
}


Image<uint16_t> openTIFF16(TIFF *tif, double gamma){
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
            TIFFReadScanline(tif, tmp.data + TIFFScanlineSize(tif) * row, row, 0);
        }

        tmp.convertTo(im, [](uint8_t num){ return num << 8; });
    }

    if (gamma > 1.0){
        for(size_t i = 0; i < im.length(); i++){
            im.data[i] = 65535.0 * pow(im.data[i] / 65535.0, 1/gamma);
        }
    }

    return im;
}



} // namespace tw
