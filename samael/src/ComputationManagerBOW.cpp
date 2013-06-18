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

ComputationManagerBOW::ComputationManagerBOW(int clusterNumber, Detector featureDetector) : m_featureDetector(featureDetector)
{
  m_bowTrainer = new cv::BOWKMeansTrainer(clusterNumber);
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
  m_vocabulary.deallocate();//resets the vocabulary if the detector changes
  m_bowTrainer->clear();//clears all previous vocabularies added to the trainer
}

void ComputationManagerBOW::getFeatureDetector(int &featureDetector)
{
  featureDetector = m_featureDetector;
}

void ComputationManagerBOW::createVocabulary(std::map<QString, std::vector<SamaelImage*>> &images)
{
  std::vector<cv::Mat> rawImageData;
  
  for(std::map<QString, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    QString className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<cv::Mat> imageDescriptors;
    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;
    imageDescriptors.resize(classImages.size());
    imageKeyPoints.resize(classImages.size());

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }

    switch(m_featureDetector)
    {
    case DETECTOR_SIFT:
      SIFT(rawImageData, imageKeyPoints, &imageDescriptors);
      break;
    case DETECTOR_SURF:
      SURF(rawImageData, imageKeyPoints, &imageDescriptors);
      break;
    case DETECTOR_MSER:
      MSER(rawImageData, imageKeyPoints, &imageDescriptors);
      break;
    };

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
  std::vector<cv::Mat> rawImageData;

  m_bowExtractor->setVocabulary(m_vocabulary);

  for(std::map<QString, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    QString className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;
    imageKeyPoints.resize(classImages.size());

    m_classNames.push_back(className);

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }

    switch(m_featureDetector)
    {
    case DETECTOR_SIFT:
      SIFT(rawImageData, imageKeyPoints);
      break;
    case DETECTOR_SURF:
      SURF(rawImageData, imageKeyPoints);
      break;
    case DETECTOR_MSER:
      MSER(rawImageData, imageKeyPoints);
      break;
    };

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
    samples.convertTo(samples_32f, CV_32F);
    if(samples.rows == 0) 
    {
      continue; //phantom class?!
    }
    CvSVM classifier; 
    classifier.train(samples_32f, labels);
    m_classifiers[className] = classifier;
  }
}

void ComputationManagerBOW::classify(std::map<QString, std::vector<SamaelImage*>> &images, std::vector<QString> &out_classNames)
{
  std::map<QString, std::map<QString, int>> confusionMatrix;
  std::vector<cv::Mat> rawImageData;

  for(std::map<QString, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    QString className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;
    imageKeyPoints.resize(classImages.size());

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }

    switch(m_featureDetector)
    {
    case DETECTOR_SIFT:
      SIFT(rawImageData, imageKeyPoints);
      break;
    case DETECTOR_SURF:
      SURF(rawImageData, imageKeyPoints);
      break;
    case DETECTOR_MSER:
      MSER(rawImageData, imageKeyPoints);
      break;
    };

    m_bowExtractor->setVocabulary(m_vocabulary);

    for(int j = 0; j < rawImageData.size(); j++)
    {
      cv::Mat histogram;
      m_bowExtractor->compute(rawImageData[j], imageKeyPoints[j], histogram);

      float minf = FLT_MAX; 
      QString minClass;

      for(std::map<QString, CvSVM>::iterator cit = m_classifiers.begin(); cit != m_classifiers.end(); cit++)
      {
        float response = cit->second.predict(histogram, true);

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

void ComputationManagerBOW::SIFT(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::SiftFeatureDetector>(new cv::SiftFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new cv::FlannBasedMatcher());
  cv::Ptr<cv::DescriptorExtractor> extractor(new cv::OpponentColorDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>(new cv::SiftDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  for(int i = 0; i < images.size(); i++)//ToDo: parallelization
  {
    detector->detect(images[i], out_imageKeyPoints[i]);//create/detect keypoints
  }

  if(out_imageDescriptors != nullptr)
  {
    for(int i = 0; i < images.size(); i++)//ToDo: parallelization
    {
      extractor->compute(images[i], out_imageKeyPoints[i], (*out_imageDescriptors)[i]);//create keypoint descriptors
    }
  }
}

void ComputationManagerBOW::SURF(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::SurfFeatureDetector>(new cv::SurfFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new cv::FlannBasedMatcher());
  cv::Ptr<cv::DescriptorExtractor> extractor(new cv::OpponentColorDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>(new cv::SurfDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  for(int i = 0; i < images.size(); i++)//ToDo: parallelization
  {
    detector->detect(images[i], out_imageKeyPoints[i]);//create/detect keypoints
  }

  if(out_imageDescriptors != nullptr)
  {
    for(int i = 0; i < images.size(); i++)//ToDo: parallelization
    {
      extractor->compute(images[i], out_imageKeyPoints[i], (*out_imageDescriptors)[i]);//create keypoint descriptors
    }
  }
}

void ComputationManagerBOW::MSER(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::MserFeatureDetector>(new cv::MserFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new cv::FlannBasedMatcher());
  cv::Ptr<cv::DescriptorExtractor> extractor(new cv::OpponentColorDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>(new cv::SiftDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  for(int i = 0; i < images.size(); i++)//ToDo: parallelization
  {
    detector->detect(images[i], out_imageKeyPoints[i]);//create/detect keypoints
  }

  if(out_imageDescriptors != nullptr)
  {
    for(int i = 0; i < images.size(); i++)//ToDo: parallelization
    {
      extractor->compute(images[i], out_imageKeyPoints[i], (*out_imageDescriptors)[i]);//create keypoint descriptors
    }
  }
}