#ifndef SAMAELIMAGE_H
#define SAMAELIMAGE_H

#include <opencv2/core/core.hpp>

class SamaelImage
{
public:
    SamaelImage();
    SamaelImage(const QFileInfo info);
    ~SamaelImage();

    QFileInfo& getFileInfo();
    cv::Mat& getMat();

private:
    QFileInfo m_FileInfo;
    cv::Mat m_Mat;
};
Q_DECLARE_METATYPE(SamaelImage)

#endif // SAMAELIMAGE_H
