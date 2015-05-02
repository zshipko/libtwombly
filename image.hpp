#ifndef TWOMBLY_IMAGE_HEADER
#define TWOMBLY_IMAGE_HEADER
#include "extra/all"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <functional>
#include <iostream>


namespace tw {
using namespace extra;

typedef int64_t ImageSizeType;

class Point {
public:
    ImageSizeType x, y;
    Point(ImageSizeType _x, ImageSizeType _y) : x(_x), y(_y){}
    Point() : x(0), y(0) {}
};

class Rectangle {
public:
    ImageSizeType x, y, width, height;
    Rectangle(ImageSizeType _x, ImageSizeType _y, ImageSizeType _w, ImageSizeType _h) : x(_x), y(_y), width(_w), height(_h) {}
};

template <typename DataType>
class Image {
    bool ownsdata;
public:
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
        if (ownsdata){
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
        rowstride() * height;
    }

    static inline size_t datasize(){
        return sizeof(DataType);
    }

    size_t offs(ImageSizeType x, ImageSizeType y){
        return (y * rowstride()) + (x * channels);
    }

    DataType &operator[](size_t index){
        return data[index];
    }

    DataType &operator()(ImageSizeType x, ImageSizeType y, ImageSizeType c){
        return data[offs(x, y) + c];
    }

    DataType *operator()(ImageSizeType x, ImageSizeType y){
        return data+offs(x, y);
    }

    Image<DataType>& operator=(Image<DataType> const &src){
        if (valid()){
            delete[] data;
        }

        width = src.width;
        height = src.height;
        channels = src.channels;
        data = src.data;
        ownsdata = src.ownsdata;
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
};

} //namespace tw

#endif //TWOMBLY_IMAGE_HEADER
