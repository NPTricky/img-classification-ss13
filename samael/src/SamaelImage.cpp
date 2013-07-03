#include "stdafx.h"
#include "SamaelImage.h"
#include "SamaelUtility.h"
#include "Texture.h"

SamaelImage::SamaelImage()
{

}

SamaelImage::SamaelImage(const QString& path)
{
    cv::Mat tmpMat = cv::imread(path.toStdString(), CV_LOAD_IMAGE_COLOR);
    m_Mat.create(tmpMat.rows, tmpMat.cols, CV_8UC3);
	tmpMat.convertTo(m_Mat, CV_8UC3);
    //m_texture = new Texture(m_Mat.cols, m_Mat.rows, GL_TEXTURE_2D, GL_UNSIGNED_BYTE, GL_RGB8, GL_RGB, m_Mat.data);//buggy, maybe wrong format
}

SamaelImage::~SamaelImage()
{
  //delete m_texture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Getters & Setters
///////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat& SamaelImage::getMat()
{
    return m_Mat;
}

Texture& SamaelImage::getTexture()
{
  return *m_texture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////////////////////////

