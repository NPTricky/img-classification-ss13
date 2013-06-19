#include "stdafx.h"
#include "ComputationManagerBOW.h"

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include <QDebug>
#include <limits>
#include <QVector4d>

ComputationManagerBOW::ComputationManagerBOW(int clusterNumber, Detector featureDetector)
{
  m_bowTrainer = new cv::BOWKMeansTrainer(clusterNumber);
  m_detector = nullptr;
  m_matcher = nullptr;
  m_extractor = nullptr;
  m_bowExtractor = nullptr;

  setFeatureDetector(featureDetector);
}

ComputationManagerBOW::~ComputationManagerBOW()
{
  delete m_bowTrainer;
  delete m_bowExtractor;
}

ComputationManagerBOW* ComputationManagerBOW::getInstance(int clusterNumber, Detector featureDetector)
{
  static ComputationManagerBOW instance(clusterNumber, featureDetector);
  return &instance;
}

void ComputationManagerBOW::setFeatureDetector(int featureDetector)
{
  m_featureDetector = Detector(featureDetector);
  m_bowTrainer->clear();//clears all previous vocabularies added to the trainer

  if(!m_vocabulary.empty())
  {
    m_vocabulary.deallocate();//resets the vocabulary if the detector changes
  }

  switch(m_featureDetector)
  {
  case DETECTOR_SIFT:
    SIFT();
    break;
  case DETECTOR_SURF:
    SURF();
    break;
  case DETECTOR_MSER:
    MSER();
    break;
  };
}

void ComputationManagerBOW::getFeatureDetector(int &featureDetector)
{
  featureDetector = m_featureDetector;
}

void ComputationManagerBOW::createVocabulary(std::map<QString, std::vector<SamaelImage*>> &images)
{
  for(std::map<QString, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    QString className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<cv::Mat> rawImageData;
    std::vector<cv::Mat> imageDescriptors;
    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;

    imageDescriptors.resize(classImages.size());
    imageKeyPoints.resize(classImages.size());

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }
    
    detect(rawImageData, imageKeyPoints, &imageDescriptors);

    for(int i = 0; i < imageDescriptors.size(); i++)
    {
      m_bowTrainer->add(imageDescriptors[i]);
    }
  }

  m_vocabulary = m_bowTrainer->cluster();
}

void ComputationManagerBOW::trainClassifier(std::map<QString, std::vector<SamaelImage*>> &images)
{
  m_classNames.clear();
  
  m_bowExtractor->setVocabulary(m_vocabulary);

  for(std::map<QString, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    QString className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<cv::Mat> rawImageData;
    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;

    imageKeyPoints.resize(classImages.size());

    m_classNames.push_back(className);

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }

    detect(rawImageData, imageKeyPoints);

    cv::Mat histogram;

    for(int j = 0; j < rawImageData.size(); j++)
    {
      m_bowExtractor->compute(rawImageData[j], imageKeyPoints[j], histogram);

      if(m_histograms.count(className) == 0)
      {
        m_histograms[className].create(0, histogram.cols, histogram.type());
      }
      m_histograms[className].push_back(histogram);
    }
  }
}

void ComputationManagerBOW::trainSVM()
{
  for(int i = 0; i < m_classNames.size(); i++)
  {
    QString className = m_classNames[i];

    cv::Mat samples(0, m_histograms[className].cols, m_histograms[className].type());
    cv::Mat labels(0, 1, CV_32FC1);

    samples.push_back(m_histograms[className]);
    cv::Mat classLabel = cv::Mat::ones(m_histograms[className].rows, 1, CV_32FC1);
    labels.push_back(classLabel);

    for(std::map<QString, cv::Mat>::iterator hit = m_histograms.begin(); hit != m_histograms.end(); hit++)
    {
      QString notClassName = hit->first;

      if(className == notClassName)
      {
        continue;
      }

      samples.push_back(m_histograms[notClassName]);
      classLabel = cv::Mat::zeros(m_histograms[className].rows, 1, CV_32FC1);
      labels.push_back(classLabel);
    }

    cv::Mat samples_32f; 
    samples.convertTo(samples_32f, CV_32FC1);
    if(samples.rows == 0) 
    {
      continue; //phantom class?!
    }
    CvSVM *classifier = new CvSVM; 
    classifier->train(samples_32f, labels);
    m_classifiers[className] = classifier;
  }
}

void ComputationManagerBOW::classify(std::map<QString, std::vector<SamaelImage*>> &images, std::vector<QString> &out_classNames)
{
  std::map<QString, std::map<QString, int>> confusionMatrix;

  for(std::map<QString, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    QString className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<cv::Mat> rawImageData;
    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;

    imageKeyPoints.resize(classImages.size());

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }

    detect(rawImageData, imageKeyPoints);

    m_bowExtractor->setVocabulary(m_vocabulary);

    for(int j = 0; j < rawImageData.size(); j++)
    {
      cv::Mat histogram;
      m_bowExtractor->compute(rawImageData[j], imageKeyPoints[j], histogram);

      float minf = FLT_MAX; 
      QString minClass;

      for(std::map<QString, CvSVM*>::iterator cit = m_classifiers.begin(); cit != m_classifiers.end(); cit++)
      {
        float response = cit->second->predict(histogram, true);

        if(response < minf)
        {
          minf = response;
          minClass = cit->first;
        }
      
      }
      confusionMatrix[minClass][className]++;
    }

    out_classNames = std::vector<QString>();
  }
}

void ComputationManagerBOW::detect(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors)
{
  for(int i = 0; i < images.size(); i++)//ToDo: parallelization
  {
    m_detector->detect(images[i], out_imageKeyPoints[i]);//create/detect keypoints
  }

  if(out_imageDescriptors != nullptr)
  {
    for(int i = 0; i < images.size(); i++)//ToDo: parallelization
    {
      m_extractor->compute(images[i], out_imageKeyPoints[i], (*out_imageDescriptors)[i]);//create keypoint descriptors
    }
  }
}

void ComputationManagerBOW::SIFT()
{
  if(m_matcher != nullptr)
  {
    delete m_matcher;
  }

  if(m_extractor != nullptr)
  {
    delete m_extractor;
  }

  if(m_bowExtractor != nullptr)
  {
    delete m_bowExtractor;
  }

  m_detector = cv::Ptr<cv::SiftFeatureDetector>(new cv::SiftFeatureDetector());
  m_matcher = new cv::FlannBasedMatcher();
  m_extractor = new cv::OpponentColorDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>(new cv::SiftDescriptorExtractor()));
  m_bowExtractor = new cv::BOWImgDescriptorExtractor(m_extractor, m_matcher);
}

void ComputationManagerBOW::SURF()
{
  if(m_matcher != nullptr)
  {
    delete m_matcher;
  }

  if(m_extractor != nullptr)
  {
    delete m_extractor;
  }

  if(m_bowExtractor != nullptr)
  {
    delete m_bowExtractor;
  }

  m_detector = cv::Ptr<cv::SurfFeatureDetector>(new cv::SurfFeatureDetector());
  m_matcher = new cv::FlannBasedMatcher();
  m_extractor = new cv::OpponentColorDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>(new cv::SurfDescriptorExtractor()));
  m_bowExtractor = new cv::BOWImgDescriptorExtractor(m_extractor, m_matcher);
}

void ComputationManagerBOW::MSER()
{
  if(m_matcher != nullptr)
  {
    delete m_matcher;
  }

  if(m_extractor != nullptr)
  {
    delete m_extractor;
  }

  if(m_bowExtractor != nullptr)
  {
    delete m_bowExtractor;
  }

  m_detector = cv::Ptr<cv::MserFeatureDetector>(new cv::MserFeatureDetector());
  m_matcher = new cv::FlannBasedMatcher();
  m_extractor = new cv::OpponentColorDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>(new cv::SurfDescriptorExtractor()));
  m_bowExtractor = new cv::BOWImgDescriptorExtractor(m_extractor, m_matcher);
}