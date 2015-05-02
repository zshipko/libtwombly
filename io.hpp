#ifndef TWOMBLY_IO_HEADER
#define TWOMBLY_IO_HEADER

#include "image.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

namespace tw {

#ifndef NO_TIFF
#include <tiff.h>
#include <tiffio.h>
bool saveTIFF(Image<uint8_t> &im, const char *path, const char *mode = "w");
bool saveTIFF(Image<uint16_t> &im, const char *path, const char *mode = "w");
TIFF *tiffOpen(const char *path, uint16_t *channels = nullptr, uint32_t *depth = nullptr);
Image<uint8_t> openTIFF8(TIFF *tif, double gamma = 0);
Image<uint16_t> openTIFF16(TIFF *tif, double gamma = 0);
#endif

bool imread(const char *path, Image<uint16_t> &im);
bool imread(const char *path, Image<uint8_t> &im);

bool imwrite(Image<uint8_t> &, const char *);
bool imwrite(Image<uint16_t> &, const char *);

} // namespace tw

#endif // TWOMBLY_IO_HEADER
