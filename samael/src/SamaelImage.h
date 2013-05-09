#ifndef SAMAELIMAGE_H
#define SAMAELIMAGE_H

#include <opencv2/core/core.hpp>

class SamaelImage
{
public:
    SamaelImage();
    SamaelImage(const QString& path);
    ~SamaelImage();

    cv::Mat& getMat();

private:
    cv::Mat m_Mat;
};
Q_DECLARE_METATYPE(SamaelImage)

#endif // SAMAELIMAGE_H
