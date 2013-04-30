#ifndef SAMAELIMAGEITEM_H
#define SAMAELIMAGEITEM_H

#include "opencv2\core\core.hpp"

class SamaelImage
{
public:
    SamaelImage();
    ~SamaelImage();

private:
    QFileInfo& m_FileInfo;
    cv::Mat* m_Mat;
};
Q_DECLARE_METATYPE(SamaelImage)

#endif // SAMAELIMAGEITEM_H
