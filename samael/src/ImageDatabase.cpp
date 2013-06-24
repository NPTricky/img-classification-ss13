#include "stdafx.h"
#include "ImageDatabase.h"

ImageDataBase::ImageDataBase()
{
}

ImageDataBase::~ImageDataBase()
{
}

ImageDataBase* ImageDataBase::getInstance()
{
  static ImageDataBase imageDataBase;
  return &imageDataBase;
}

void ImageDataBase::addImage(std::string className, SamaelImage *image)
{
  bool contained = false;
  for(int i = 0; i < m_classNames.size(); i++)
  {
    if(!m_classNames[i].compare(className))
    {
      contained = true;
      break;
    }
  }

  if(!contained)
  {
    m_classNames.push_back(className);
  }

  m_images[className].push_back(image);
}

void ImageDataBase::removeImages(std::string className)
{
  m_images[className].clear();

  for(std::vector<std::string>::iterator it = m_classNames.begin(); it != m_classNames.end(); it++)
  {
    if(!it->compare(className))
    {
      m_classNames.erase(it);
      break;
    }
  }
}

void ImageDataBase::getClassNames(std::vector<std::string> &out_classNames)
{
  out_classNames = m_classNames;
}

void ImageDataBase::getTrainingImages(std::map<std::string, std::vector<SamaelImage*>> &out_images)
{
  for(int j = 0; j < m_classNames.size(); j++)
  {
    std::string className = m_classNames[j];
    m_trainingImageIndices[className].clear();

    std::map<int, bool> indices; 
    int index;
    int size = m_images[className].size();

    for(int i = 0; i < size / 2 + (size & 1); i++)//choose 50% random images for training and save their indices
    {
      index = rand() % size;//random index

      if(!indices[index])//if the index hasn't been choosen before...
      {
        indices[index] = true;//... take it now
      }
      else
      {
        for(; index < size; index++)//... or take the closest index near him
        {
          if(!indices[index])
          {
            indices[index] = true;
            break;
          }

          if(index + 1 == size)//if we reached the end, go back to the beginning
          {
            index = 0;
          }
        }
      }

      m_trainingImageIndices[className].push_back(index);//save the index of the training image
    }

    out_images[className].resize(m_trainingImageIndices[className].size());

    for(int i = 0; i < m_trainingImageIndices[className].size(); i++)
    {
      out_images[className][i] = m_images[className][m_trainingImageIndices[className][i]];//copy all images from the training into the output
    }
  }
}

void ImageDataBase::getClassifyImages(std::map<std::string, std::vector<SamaelImage*>> &out_images)
{
  for(int j = 0; j < m_classNames.size(); j++)
  {
    std::string className = m_classNames[j];
    out_images[className].resize(m_trainingImageIndices[className].size() - (m_images[className].size() & 1));

    bool copy = true;
    int offset = 0;
    for(int i = 0; i < m_images[className].size(); i++)
    {
      for(int j = 0; j < m_trainingImageIndices[className].size(); j++)
      {
        if(m_trainingImageIndices[className][j] == i)
        {
          copy = false;
          offset++;
          break;
        }
      }

      if(copy)
      {
        out_images[className][i - offset] = m_images[className][i];//copy all images which are not used for the training into the output
      }
      copy = true;
    }
  }
}