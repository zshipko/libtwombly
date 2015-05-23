#ifndef TWOMBLY_LAYERS_HEADER
#define TWOMBLY_LAYERS_HEADER

#include "draw.hpp"

namespace tw {

template <typename LayersImageType>
class Layers : public list<LayersImageType*> {

public:
    size_t length(){
        return this->size();
    }

    void append(LayersImageType *im, Point p){
        this->push_back(im);
        this->operator[](this->size()-1)->origin = p;
    }

    void operator+=(LayersImageType *t){
        this->push_back(t);
    }

    template <typename RemoveFunction>
    void removeIf(RemoveFunction func){
        this->erase(std::remove_if(this->begin(), this->end(), func), this->end());
    }

    Point maxSize(){
        Point p;

        for(auto &im : *this){
            if (im->width + im->origin.x > p.x){
                p.x = im->width + im->origin.x;
            }

            if (im->height + im->origin.y > p.y){
                p.y = im->height + im->origin.y;
            }
        }

        return p;
    }

    LayersImageType flatten(){
        Point p = maxSize();
        LayersImageType dst(p.x, p.y);
        for(auto &im : *this){
            dst(im->origin, *im);
        }

        return dst;
    }

    LayersImageType &get(size_t index){
        return *(this->operator[](index));
    }
};

typedef Layers<Image8> Layers8;
typedef Layers<Image16> Layers16;
typedef Layers<Image32> Layers32;

} // namespace tw

#endif // TWOMBLY_LAYERS_HEADER
