#ifndef SAMAELUTILITY_H
#define SAMAELUTILITY_H

#include <opencv2/core/core.hpp>

namespace SamaelUtility
{
    QImage MatToQImage(const cv::Mat3b &src); ///< 24 Bit 888 RGB -> QImage
    QImage MatToQImage(const cv::Mat4b &src); ///< 32 Bit 8888 RGBA -> QImage
    QImage MatToQImage(const cv::Mat_<double> &src); ///< Grayscale (Floating Point) -> QImage

    template <typename T>
    cv::Mat_<T> ResizeMatToWidth(const cv::Mat_<T> &src, const int width);
}

#endif // SAMAELUTILITY_H
