#ifndef SAMAELIMAGE_H
#define SAMAELIMAGE_H

#include <opencv2/core/core.hpp>

class Texture;

class SamaelImage
{
public:
    SamaelImage();
    SamaelImage(const QString& path);
    ~SamaelImage();

    cv::Mat& getMat();
    Texture& getTexture();

private:
    cv::Mat m_Mat;
    Texture *m_texture;
};
Q_DECLARE_METATYPE(SamaelImage)

#endif // SAMAELIMAGE_H
