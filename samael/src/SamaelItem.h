#ifndef SAMAELIMAGE_H
#define SAMAELIMAGE_H

#include "opencv2\core\core.hpp"

class SamaelItem
{
public:
    SamaelItem(QFileInfo& info);
    ~SamaelItem();

private:
    QFileInfo& m_FileInfo;
    cv::Mat* m_Mat;
};

#endif // SAMAELIMAGE_H
