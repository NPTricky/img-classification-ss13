#ifndef SAMAELUTILITY_H
#define SAMAELUTILITY_H

#include "opencv2\core\core.hpp"

namespace SAM
{
    QImage MatToQImage(const cv::Mat3b &src); ///< 24 Bit 888 RGB -> QImage
    QImage MatToQImage(const cv::Mat4b &src); ///< 32 Bit 8888 RGBA -> QImage
    QImage MatToQImage(const cv::Mat_<double> &src); ///< Grayscale (Floating Point) -> QImage
}

#endif // SAMAELUTILITY_H
