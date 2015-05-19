#ifndef TWOMBLY_LAYERS_HEADER
#define TWOMBLY_LAYERS_HEADER

#include "draw.hpp"

namespace tw {
template <typename LayersImageType>
class Layers {

public:
    typedef std::tuple<LayersImageType, Point> layer;

    Layers(){}

    void push_back(LayersImageType &im, Point p = Point()){
        auto t = std::make_tuple(im, p);
        arr.push_back(t);
    }

    void push_back(layer t){
        arr.push_back(t);
    }

    LayersImageType &image(size_t index){
        if (index >= arr.size()){
            throw std::runtime_error("out of bounds");
        }
        auto item = arr[index];
        return std::get<0>(item);
    }

    LayersImageType &operator[](size_t index){
        return image(index);
    }

    Point &point(size_t index){
        if (index >= arr.size()){
            throw std::runtime_error("out of bounds");
        }
        auto item = arr[index];
        return std::get<1>(item);
    }

    void operator+=(layer t){
        push_back(t);
    }

    typename list<layer>::iterator begin(){
        return arr.begin();
    }

    typename list<layer>::iterator end(){
        return arr.end();
    }

    typename list<layer>::iterator begin() const {
        return arr.begin();
    }

    typename list<layer>::iterator end() const {
        return arr.end();
    }

    typename list<layer>::reverse_iterator rbegin(){
        return arr.rbegin();
    }

    typename list<layer>::reverse_iterator rend(){
        return arr.rend();
    }

    list<layer> &operator()(){
        return arr;
    }

    list<layer> &iter(){
        return arr;
    }

    template <typename RemoveFunction>
    void removeIf(RemoveFunction func){
        arr.erase(std::remove_if(arr.begin(), arr.end(), func), arr.end());
    }

    Point sizeMax(){
        Point p;

        for(auto &i : arr){
            auto &im = std::get<0>(i);
            auto &pt = std::get<1>(i);
            if (im.width + pt.x > p.x){
                p.x = im.width + pt.x;
            }

            if (im.height + pt.y > p.y){
                p.y = im.height + pt.y;
            }
        }

        return p;
    }

    LayersImageType flatten(){
        Point p = sizeMax();
        LayersImageType dst(p.x, p.y);
        for(auto &i : arr){
            auto &im = std::get<0>(i);
            auto &pt = std::get<1>(i);
            for(int32_t y = pt.y; y < pt.y + im.height; y++){
                memcpy((void*)(dst.data + dst.offs(pt.x, y)), im.at(0, y - pt.y), sizeof(im.data[0]) * im.width);
            }
        }

        return dst;
    }

private:
    list<layer> arr;
};

} // namespace tw

#endif // TWOMBLY_LAYERS_HEADER
