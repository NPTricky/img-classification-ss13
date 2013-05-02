#include "stdafx.h"
#include "SamaelUtility.h"

namespace SamaelUtility
{

    QImage MatToQImage(const cv::Mat3b &src) 
    {
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y)
        {
            const cv::Vec3b *srcrow = src[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < src.cols; ++x)
            {
                destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
            }
        }
        return dest;
    }

    QImage MatToQImage(const cv::Mat4b &src) 
    {
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y)
        {
            const cv::Vec4b *srcrow = src[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < src.cols; ++x)
            {
                destrow[x] = qRgba(srcrow[x][3], srcrow[x][2], srcrow[x][1], srcrow[x][0]);
            }
        }
        return dest;
    }

    QImage MatToQImage(const cv::Mat_<double> &src)
    {
        double scale = 255.0;
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y)
        {
            const double *srcrow = src[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < src.cols; ++x)
            {
                unsigned int intensity = srcrow[x] * scale;
                destrow[x] = qRgba(intensity, intensity, intensity, 255);
            }
        }
        return dest;
    }

    template <typename T>
    cv::Mat_<T> ResizeMatToWidth(const cv::Mat_<T> &src, const int width)
    {
        int rows = cvRound(src.rows/src.cols) * width;
        cv::Mat_<T> dest(rows,width);
        cv::resize(src, dest, dest.size(), 0, 0, cv::INTER_LINEAR);
        return dest;
    }

} // namespace SamaelUtility