#ifndef TWOMBLY_IO_HEADER
#define TWOMBLY_IO_HEADER

#include "image.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

namespace tw {

#ifndef NO_TIFF
#include <tiff.h>
#include <tiffio.h>
TIFF *tiffOpen(const char *path, const char *mode = "r");
void tiffClose(TIFF *tif);

bool writeTIFF(TIFF *tif, Image<uint8_t> const &im);
bool writeTIFF(TIFF *tif, Image<uint16_t> const &im);
bool writeTIFF(const char *path, Image<uint8_t> const &im);
bool writeTIFF(const char *path, Image<uint16_t> const &im);

Image<uint8_t> readTIFF(TIFF *tif);
Image<uint16_t> readTIFF16(TIFF *tif);
Image<uint8_t> readTIFF(const char *path);
Image<uint16_t> readTIFF16(const char *path);
#endif

// bool imread(const char *path, Image<uint16_t> &im);
Image<uint8_t> imread(const char *path);

bool imwrite(const char *, Image<uint8_t> const &);
bool imwrite(const char *, Image<uint16> const &);

} // namespace tw

#endif // TWOMBLY_IO_HEADER
