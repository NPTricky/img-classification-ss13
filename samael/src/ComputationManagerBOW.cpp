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

using namespace std;
using namespace cv;

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
}

void ComputationManagerBOW::getFeatureDetector(int &featureDetector)
{
  featureDetector = m_featureDetector;
}

void ComputationManagerBOW::trainClassifier(QString className, std::vector<SamaelImage*> &images)
{
  std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;
  std::vector<cv::Mat> imageDescriptors;
  std::vector<cv::Mat> rawImageData;

  imageKeyPoints.resize(images.size());
  imageDescriptors.resize(images.size());
  rawImageData.resize(images.size());

  m_classVocabularies.resize(m_classVocabularies.size() + 1);

  for(int i = 0; i < images.size(); i++)
  {
    rawImageData[i] = images[i]->getMat();
  }

  switch(m_featureDetector)
  {
  case DETECTOR_SIFT:
    SIFT(rawImageData, imageKeyPoints, imageDescriptors);
    break;
  case DETECTOR_SURF:
    SURF(rawImageData, imageKeyPoints, imageDescriptors);
    break;
  case DETECTOR_MSER:
    MSER(rawImageData, imageKeyPoints, imageDescriptors);
    break;
  };

  createVocabulary(m_classNameID[className], imageDescriptors);
}

void ComputationManagerBOW::createVocabulary(int id, std::vector<cv::Mat> trainingDescriptors)
{
  for(int i = 0; i < trainingDescriptors.size(); i++)
  {
    m_bowTrainer->add(trainingDescriptors[i]);
  }

  m_classVocabularies[id] = m_bowTrainer->cluster();
}

void ComputationManagerBOW::classify(std::vector<SamaelImage*> &images, std::vector<QString> &out_classNames)
{
  std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;
  std::vector<cv::Mat> imageDescriptors;
  std::vector<cv::Mat> rawImageData;

  imageKeyPoints.resize(images.size());
  imageDescriptors.resize(images.size());
  out_classNames.resize(images.size());
  rawImageData.resize(images.size());

  for(int i = 0; i < images.size(); i++)
  {
    rawImageData[i] = images[i]->getMat();
  }

  switch(m_featureDetector)
  {
  case DETECTOR_SIFT:
    SIFT(rawImageData, imageKeyPoints, imageDescriptors);
    break;
  case DETECTOR_SURF:
    SURF(rawImageData, imageKeyPoints, imageDescriptors);
    break;
  case DETECTOR_MSER:
    MSER(rawImageData, imageKeyPoints, imageDescriptors);
    break;
  };

  std::vector<std::vector<int>> histogram;

  for(int j = 0; j < images.size(); j++)
  {
    for(int i = 0; i < m_classVocabularies.size(); i++)
    {
      m_bowExtractor->setVocabulary(m_classVocabularies[i]);
      m_bowExtractor->compute(rawImageData[j], imageKeyPoints[j], imageDescriptors[j], &histogram);
    }//TODO: auswerten zu welcher Klasse das Histogramm passt und mit der eigentlichen Klasse vergleichen
  }

  out_classNames = std::vector<QString>();
}

void ComputationManagerBOW::SIFT(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::SiftFeatureDetector>(new cv::SiftFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new BFMatcher(NORM_L2, false));
  cv::Ptr<cv::DescriptorExtractor> extractor(new OpponentColorDescriptorExtractor(Ptr<DescriptorExtractor>(new cv::SiftDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  detector->detect(images, imageKeyPoints);//create/detect keypoints
  extractor->compute(images, imageKeyPoints, imageDescriptors);//create keypoint descriptors
}

void ComputationManagerBOW::SURF(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::SurfFeatureDetector>(new cv::SurfFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new BFMatcher(NORM_L2, false));
  cv::Ptr<cv::DescriptorExtractor> extractor(new OpponentColorDescriptorExtractor(Ptr<DescriptorExtractor>(new cv::SurfDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  detector->detect(images, imageKeyPoints);//create/detect keypoints
  extractor->compute(images, imageKeyPoints, imageDescriptors);//create keypoint descriptors
}

void ComputationManagerBOW::MSER(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::MserFeatureDetector>(new cv::MserFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new BFMatcher(NORM_L2, false));
  cv::Ptr<cv::DescriptorExtractor> extractor(new OpponentColorDescriptorExtractor(Ptr<DescriptorExtractor>(new cv::SiftDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  detector->detect(images, imageKeyPoints);//create/detect keypoints
  extractor->compute(images, imageKeyPoints, imageDescriptors);//create keypoint descriptors
}