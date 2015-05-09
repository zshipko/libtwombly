#include "image.hpp"

namespace tw {
#ifdef USE_OPENCV

Image8 toImage(Mat1b &im){
    return Image8(im.cols, im.rows, 1, (uint8_t*)im.data);
}

Image8 toImage(Mat3b &im){
    return Image8(im.cols, im.rows, 3, (uint8_t*)im.data);
}

Image8 toImage(Mat4b &im){
    return Image8(im.cols, im.rows, 4, (uint8_t*)im.data);
}

Image16 toImage(Mat1w &im){
    return Image16(im.cols, im.rows, 1, (uint16_t*)im.data);
}

Image16 toImage(Mat3w &im){
    return Image16(im.cols, im.rows, 3, (uint16_t*)im.data);
}

Image16 toImage(Mat4w &im){
    return Image16(im.cols, im.rows, 4, (uint16_t*)im.data);
}

Image32 toImage(Mat1f &im){
    return Image32(im.cols, im.rows, 1, (float*)im.data);
}

Image32 toImage(Mat3f &im){
    return Image32(im.cols, im.rows, 3, (float*)im.data);
}

Image32 toImage(Mat4f &im){
    return Image32(im.cols, im.rows, 4, (float*)im.data);
}

Mat toMat(Image8 &im){
    if (im.channels == 1){
        return Mat(im.height, im.width, CV_8UC1, im.data);
    } else if (im.channels == 3){
        return Mat(im.height, im.width, CV_8UC3, im.data);
    } else if (im.channels == 4){
        return Mat(im.height, im.width, CV_8UC4, im.data);
    }

    return Mat();
}

Mat toMat(Image16 &im){
    if (im.channels == 1){
        return Mat(im.height, im.width, CV_16UC1, im.data);
    } else if (im.channels == 3){
        return Mat(im.height, im.width, CV_16UC3, im.data);
    } else if (im.channels == 4){
        return Mat(im.height, im.width, CV_16UC4, im.data);
    }

    return Mat();
}

Mat toMat(Image32 &im){
    if (im.channels == 1){
        return Mat(im.height, im.width, CV_32FC1, im.data);
    } else if (im.channels == 3){
        return Mat(im.height, im.width, CV_32FC3, im.data);
    } else if (im.channels == 4){
        return Mat(im.height, im.width, CV_32FC4, im.data);
    }

    return Mat();
}

#endif // USE_OPENCV

} // namespace tw
