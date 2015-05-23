#ifndef TWOMBLY_IMAGE_HEADER
#define TWOMBLY_IMAGE_HEADER
#include "extra/all"
#include <bitset>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "agg_trans_affine.h"

#define clamp_(a, b, c) (a < b ? b : (a > c ? c : a))
#define min_(a, b) (a < b ? a : b)
#define max_(a, b) (a > b ? a : b)

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

namespace tw {

#ifdef USE_OPENCV
using namespace cv;
#endif

using namespace extra;

typedef int64_t ImageSizeType;
typedef float Pixel __attribute__ ((vector_size (16)));

#ifndef USE_OPENCV
class Point {
public:
    ImageSizeType x, y;
    Point(ImageSizeType _x, ImageSizeType _y) : x(_x), y(_y){}
    Point() : x(0), y(0) {}
};
#endif // USE_OPENCV

class Rectangle {
public:
    ImageSizeType x, y, width, height;
    Rectangle(ImageSizeType _x, ImageSizeType _y, ImageSizeType _w, ImageSizeType _h) : x(_x), y(_y), width(_w), height(_h) {}
};

template <typename DataType>
class Image {
public:
    bool ownsdata;
    typedef DataType data_type;
    ImageSizeType width, height;
    int channels;
    Point origin;
    DataType *data;

    // Creates an empty image
    Image() : width(0), height(0), channels(0), data(nullptr), ownsdata(false){}

    // Creates a new image from width/height/channels and optional data
    Image(ImageSizeType w, ImageSizeType h, int c=3, DataType *d=nullptr, bool owner = false) :
        width(w), height(h), channels(c) {
        if (d){
            data = d;
            ownsdata = owner;
        } else {
            data = new DataType[w * h * c]();
            ownsdata = true;
        }
    }

    Image(Image<DataType> const &im) : width(im.width), height(im.height), channels(im.channels), ownsdata(false), data(im.data){}

    Image(Image<DataType> &&im) : width(im.width), height(im.height), channels(im.channels), ownsdata(im.ownsdata), data(im.data){
        im.ownsdata = false;
    }

    Image<DataType>& operator=(Image<DataType>&& src){
        if (this == &src)
            return *this;

        if (this->data && this->ownsdata){
            delete[] this->data;
        }

        this->width = src.width;
        this->height = src.height;
        this->channels = src.channels;
        this->origin = src.origin;

        this->data = src.data;
        this->ownsdata = src.ownsdata;
        src.ownsdata = false;

        return *this;
    }

    Image<DataType>& operator=(Image<DataType> const &src){
        if (this->data == src.data)
            return *this;

        if (this->data && this->ownsdata){
            delete[] this->data;
        }

        this->width = src.width;
        this->height = src.height;
        this->channels = src.channels;
        this->origin = src.origin;

        this->data = new DataType[width * height * channels];
        std::copy(src.data, src.data + this->size(), this->data);
        this->ownsdata = true;

        return *this;
    }

    ~Image(){
        if (ownsdata && data){
            delete[] data;
        }
    }

    // Returns length of a row of data
    inline const size_t rowstride(){
        return width * channels;
    }

    // Returns the size of all pixel data
    inline const size_t size(){
        return rowstride() * height * datasize();
    }

    // Returns the length of all pixel data
    inline const size_t length(){
        return rowstride() * height;
    }

    // Returns sizeof data type
    static inline const size_t datasize(){
        return sizeof(DataType);
    }

    // Returns data offset at point (x, y)
    size_t const offs(ImageSizeType x, ImageSizeType y){
        return (y * width * channels) + (x * channels);
    }

    inline const size_t offs(Point pt){
        return offs(pt.x, pt.y);
    }

    // Returns true if point (x, y) is within the image bounds
    // otherwise, returns false
    bool const inBounds(ImageSizeType x, ImageSizeType y){
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    inline const bool inBounds(Point pt){
        return inBounds(pt.x, pt.y);
    }

    // Get image data at index
    DataType &operator[](size_t index){
        return data[index];
    }

    // Get image data att offs(x, y) +  c
    DataType &operator()(ImageSizeType x, ImageSizeType y, int c){
        if (!inBounds(x, y)) throw std::runtime_error("image: out of bounds");
        return data[offs(x, y) + c];
    }

    inline DataType &operator()(Point pt, int c){
        return this->operator()(pt.x, pt.y, c);
    }

    // Get pointer to pixel at offs(x, y)
    DataType *operator()(ImageSizeType x, ImageSizeType y){
        if (!inBounds(x, y)) throw std::runtime_error("image: out of bounds");
        return data+offs(x, y);
    }

    inline DataType *operator()(Point pt){
        return this->operator()(pt.x, pt.y);
    }

    // Get float-pixel at offs(x, y)
   const Pixel get(ImageSizeType x, ImageSizeType y){
        Pixel p;
        auto ptr = data+offs(x, y);
        for (int c = 0; c < channels; c++){
            p[c] = (float)ptr[c];
        }
        return p;
    }

    inline const Pixel get(Point pt){
        return this->get(pt.x, pt.y);
    }

    // Set float-pixel at offs(x, y)
    void set(ImageSizeType x, ImageSizeType y, Pixel p){
        if (!inBounds(x, y)) return;

        auto ptr = data+offs(x, y);

        for (int c = 0; c < channels; c++){
            ptr[c] = (DataType)p[c];
        }
    }

    inline void set(Point pt, Pixel p){
        return this->set(pt.x, pt.y, p);
    }

    // Set image data with c-style array of pixel data
    void operator()(ImageSizeType x, ImageSizeType y, DataType *pixel){
        if (!inBounds(x, y)) return;
        memcpy(data + offs(x, y), pixel, channels * datasize());
    }

    inline void operator()(Point pt, DataType *pixel){
        return operator()(pt.x, pt.y, pixel);
    }

    // Set rectangle ith c-style array of pixel data
    void operator()(Rectangle r, DataType *pixels, size_t pixlen){
        size_t pixoffs = 0;
        for(ImageSizeType y = r.y; y < r.y + r.height; y++){
            for(ImageSizeType x = r.x; x < r.x + r.width; x++){
                if (!inBounds(x, y)) continue;
                for(int c = 0; c < channels; c++){
                    operator()(x, y, c) = pixels[pixoffs % pixlen];
                    pixoffs += 1;
                }
            }
        }
    }

    // Place image src at point pint (i, j)
    void operator()(ImageSizeType i, ImageSizeType j, Image<DataType> &src){
        auto minchan = min_(src.channels, channels);
        for(ImageSizeType y = i; y < i + src.height; y++){
            for(ImageSizeType x = j; x < j + src.width; x++){
                if(!inBounds(x, y)) continue;
                memcpy(data + offs(x, y), src(x-i, y-j), minchan * datasize());
            }
        }
    }

    void operator()(Point pt, Image<DataType> &src){
        return this->operator()(pt.x, pt.y);
    }


    // Creates copy of an existing image
    const Image<DataType> copy(){
        Image<DataType> dst(width, height, channels);
        std::copy(data, data + size(), dst.data);
        return dst;
    }

    // Crops an image
    const Image<DataType> crop(Rectangle r){
        Image<DataType> dst(r.width, r.height, channels);
        for(ImageSizeType y = r.y; y < r.y + r.height; y++){
            memcpy(dst.data + dst.offs(0, (y-r.y)), data + offs(r.x, y),
                dst.rowstride() * dst.datasize());
        }
        return dst;
    }


    inline const Image<DataType> crop(ImageSizeType x, ImageSizeType y, ImageSizeType w, ImageSizeType h){
        return this->crop(Rectangle(x, y, w, h));
    }

    // Checks if image is valid
    bool const valid(){
        return channels > 0 && channels <= 4 && width > 0 && height > 0 && data != nullptr;
    }

    DataType *begin(){
        return data;
    }

    DataType *end(){
        return data + (width * height * channels);
    }

    // Converts to a different image type using a conversion function for each pixel
    template <typename OutputImage>
    const void convertTo(OutputImage &im, std::function<typename OutputImage::data_type(DataType)> fn){
        if (im.width != width || im.height != height){
            return;
        }

        for (size_t i = 0; i < im.width * im.height * im.channels; i++){
            im.data[i] = fn(data[i]);
        }
    }

    // Converts to a different image type using min, max and scale
    template <typename OutputImage>
    const void convertTo(OutputImage &im, typename OutputImage::data_type mn = 0, typename OutputImage::data_type mx = 0, float scale = 1.0){
        if (im.width != width || im.height != height){
            return;
        }

        for (size_t i = 0; i < im.width * im.height * im.channels; i++){
            if (mn >= mx){
                im.data[i] = (typename OutputImage::data_type) data[i];
                continue;
            }

            im.data[i] = data[i] * scale > mx ? mx :
                        (data[i] * scale < mn ? mn :
                           (typename OutputImage::data_type)(data[i] * scale));
        }
    }

    // Get a single channel image from a multi-channel image,
    // throws an error if the component requested is less than the
    // number of channels in the image
    const Image<DataType> component(int c){
        if (channels <= c){
            throw std::runtime_error("image.component: bad value");
        }

        Image<DataType> dst(width, height, 1);
        for(size_t i = c; i < width * height * channels; i+=channels){
            dst[i/channels] = data[i];
        }

        return dst;
    }

    void component(int c, Image<DataType> &im){
        if (channels <= c){
            throw std::runtime_error("image.component: bad value");
        }

        for(size_t i = c; i < width * height * channels; i+=channels){
            data[i] = im[i / channels];
        }
    }

    // Convert the image to grayscle using R=0.3, G=0.59, B=0.11,
    // Image must be in RGB order
    Image<DataType> grayscale(){
        if (channels == 1){
            return copy();
        }

        Image<DataType> dst(width, height, 1);
        for(size_t i = 0; i < length(); i+=channels){
            dst[i/channels] = data[i] * 0.3 + data[i+1] * 0.59 + data[i+2] * 0.11;
        }
        return dst;
    }

    Image<DataType> &gamma(float g, float max_num = 255.0){
        if (datasize() == 2)
            max_num = 65535.0;
        else if (datasize() == 32)
            max_num = 1.0;

        for(size_t i = 0; i < length(); i++){
            //data[i] = (DataType)(max_num * pow(data[i] / max_num, 1/g));

        }

        return *this;
    }

    // Scale image to size (width*x, height*y)
    const Image<DataType> scale(double x, double y){
        Image<DataType> scaled((double)width * x, (double)height * y, channels);
        agg::trans_affine transform;
        transform.scale(x, y);

        for(double j = 0; j < scaled.height; j++){
            for(double i = 0; i < scaled.width; i++){
                double ii = i, jj = j;
                transform.inverse_transform(&ii, &jj);
                scaled(i, j, operator()(ii, jj));
            }
        }

        return scaled;
    }


    // Rotate an image around the center, angle is in radians
    const Image<DataType> rotate(float angle, ImageSizeType w = 0, ImageSizeType h = 0){
        Image<DataType> rot(w ? w : width, h ? h : height, channels);

        agg::trans_affine transform;
        transform.translate(-width/2, -height/2);
        transform.rotate(angle);
        transform.translate(rot.width/2, rot.height/2);

        for(double j = 0; j < rot.height; j++){
            for(double i = 0; i < rot.width; i++){
                double ii = i, jj = j;
                transform.transform(&ii, &jj);
                if (!rot.inBounds(ii, jj)) continue;
                for (int c = 0; c < channels; c++){
                    rot(i, j, c) = operator()(ii, jj, c);
                }
            }
        }

        return rot;
    }

    // Select a random point from the image
    const Point randomPoint(){
        return Point(arc4random_uniform(width), arc4random_uniform(height));
    }

    // Generate a simple content-based hash
    std::bitset<64> hash(){
        std::bitset<64> h = 0;
        auto tmp = this->grayscale().scale(8.0/this->width, 8.0/this->height);

        float avg = 0;
        for(auto i : tmp){
            avg += i;
        }
        avg /= 64;

        size_t bitno = 0;
        for (auto i : tmp){
            h.set(bitno, i > avg);
            bitno++;
        }

        return h;
    }
};

typedef Image<uint8_t> Image8;
typedef Image<uint16_t> Image16;
typedef Image<float> Image32;

#ifdef USE_OPENCV

Image8 toImage(Mat1b &im);
Image8 toImage(Mat3b &im);
Image8 toImage(Mat4b &im);
Image16 toImage(Mat1w &im);
Image16 toImage(Mat3w &im);
Image16 toImage(Mat4w &im);
Image32 toImage(Mat1f &im);
Image32 toImage(Mat3f &im);
Image32 toImage(Mat4f &im);

Mat toMat(Image8 &im);
Mat toMat(Image16 &im);
Mat toMat(Image32 &im);

template<typename ImageType, typename MatType>
ImageType toImage(MatType &m){
    return toImage(m);
}

template<typename MatType, typename ImageType>
MatType toMat(ImageType &im){
    return toMat(im);
}

#endif

} //namespace tw

#endif //TWOMBLY_IMAGE_HEADER
