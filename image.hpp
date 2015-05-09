#ifndef TWOMBLY_IMAGE_HEADER
#define TWOMBLY_IMAGE_HEADER
#include "extra/all"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <functional>
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
    DataType *data;
    Image() : width(0), height(0), channels(0), data(nullptr), ownsdata(false){}
    Image(Image const &im) : width(im.width), height(im.height), channels(im.channels), ownsdata(false), data(im.data){}
    Image(ImageSizeType w, ImageSizeType h, int c=3, DataType *d=nullptr) :
        width(w), height(h), channels(c) {
        if (d){
            data = d;
            ownsdata = false;
        } else {
            data = new DataType[w * h * c];
            ownsdata = true;
        }
    }

    ~Image(){
        if (ownsdata && data){
            delete[] data;
        }
    }

    inline size_t rowstride(){
        return width * channels;
    }

    inline size_t size(){
        return rowstride() * height * sizeof(DataType);
    }

    inline size_t length(){
        return rowstride() * height;
    }

    static inline size_t datasize(){
        return sizeof(DataType);
    }

    size_t offs(ImageSizeType x, ImageSizeType y){
        return (y * width * channels) + (x * channels);
    }

    inline size_t offs(Point pt){
        return offs(pt.x, pt.y);
    }

    bool inBounds(ImageSizeType x, ImageSizeType y){
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    inline bool inBounds(Point pt){
        return inBounds(pt.x, pt.y);
    }

    DataType &operator[](size_t index){
        return data[index];
    }

    DataType &operator()(ImageSizeType x, ImageSizeType y, int c){
        if (!inBounds(x, y)) throw std::runtime_error("image: out of bounds");
        return data[offs(x, y) + c];
    }

    inline DataType &operator()(Point pt, int c){
        return this->operator()(pt.x, pt.y, c);
    }

    DataType *operator()(ImageSizeType x, ImageSizeType y){
        if (!inBounds(x, y)) throw std::runtime_error("image: out of bounds");
        return data+offs(x, y);
    }

    inline DataType *operator()(Point pt){
        return this->operator()(pt.x, pt.y);
    }

    Pixel get(ImageSizeType x, ImageSizeType y){
        Pixel p;
        auto ptr = data+offs(x, y);
        for (int c = 0; c < channels; c++){
            p[c] = (float)ptr[c];
        }
        return p;
    }

    inline Pixel get(Point pt){
        return this->get(pt.x, pt.y);
    }

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

    void operator()(ImageSizeType x, ImageSizeType y, DataType *pixel){
        if (!inBounds(x, y)) return;
        memcpy(data + offs(x, y), pixel, channels * datasize());
    }

    inline void operator()(Point pt, DataType *pixel){
        return operator()(pt.x, pt.y, pixel);
    }

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

    void operator()(Point pt, Image<DataType> &src){
        auto minchan = min_(src.channels, channels);
        for(ImageSizeType y = pt.y; y < pt.y + src.height; y++){
            for(ImageSizeType x = pt.x; x < pt.x + src.width; x++){
                if(!inBounds(x, y)) continue;
                memcpy(data + offs(x, y), src(x-pt.x, y-pt.y), minchan * datasize());
            }
        }
    }

    Image<DataType>& operator=(Image<DataType>&& src){
        if (!src.valid())
            return *this;

        if (src.data == data)
            return *this;

        if (valid() && data){
            delete[] data;
        }

        width = src.width;
        height = src.height;
        channels = src.channels;
        data = src.data;
        ownsdata = src.ownsdata;
        src.ownsdata = false;
        return *this;
    }

    Image<DataType> copy(){
        Image<DataType> dst(width, height, channels);
        memcpy(dst.data, data, size());
        return dst;
    }

    Image<DataType> crop(Rectangle r){
        Image<DataType> dst(r.width, r.height, channels);
        for(ImageSizeType y = r.y; y < r.y + r.height; y++){
            memcpy(dst.data + dst.offs(0, (y-r.y)), data + offs(r.x, y),
                dst.rowstride() * dst.datasize());
        }
        return dst;
    }

    bool valid(){
        return data != nullptr;
    }

    DataType *begin(){
        return data;
    }

    DataType *end(){
        return data + (width * height * channels);
    }

    template <typename OutputImage>
    void convertTo(OutputImage &im, std::function<typename OutputImage::data_type(DataType)> fn){
        if (im.width != width || im.height != height){
            return;
        }

        for (size_t i = 0; i < im.width * im.height * im.channels; i++){
            im.data[i] = fn(data[i]);
        }
    }

    template <typename OutputImage>
    void convertTo(OutputImage &im, typename OutputImage::data_type mn = 0, typename OutputImage::data_type mx = 0, float scale = 1.0){
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

    Image<DataType> component(int c){
        Image<DataType> dst(width, height, 1);
        for(size_t i = c; i < width * height * channels; i+=channels){
            dst[i/channels] = data[i];
        }
        return dst;
    }

    void component(int c, Image<DataType> &im){
        for(size_t i = c; i < width * height * channels; i+=channels){
            data[i] = im[i / channels];
        }
    }

    Image<DataType> grayscale(){
        if (channels == 1){
            return copy();
        }

        Image<DataType> dst(width, height, 1);
        for(size_t i = 0; i < width * height * channels; i+=channels){
            dst[i/channels] = data[i] * 0.3 + data[i+1] * 0.59 + data[i+2] * 0.11;
        }
        return dst;
    }

    Image<DataType> scale(double x, double y){
        Image<DataType> scaled(width * x, height * y, channels);
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

    Image<DataType> rotate(float angle, ImageSizeType w = 0, ImageSizeType h = 0){
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

    Point randomPoint(){
        return Point(arc4random_uniform(width), arc4random_uniform(height));
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
