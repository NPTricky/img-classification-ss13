#include "stdafx.h"
#include "ComputationManager.h"

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

ComputationManager::ComputationManager(int clusterNumber, Detector featureDetector) : m_featureDetector(featureDetector)
{
  m_bowTrainer = new cv::BOWKMeansTrainer(clusterNumber);
}

ComputationManager::~ComputationManager()
{
  delete m_bowTrainer;
  delete m_bowExtractor;
}

ComputationManager* ComputationManager::getInstance(int clusterNumber, Detector featureDetector)
{
  static ComputationManager instance(clusterNumber, featureDetector);
  return &instance;
}

void ComputationManager::setFeatureDetector(int featureDetector)
{
  m_featureDetector = Detector(featureDetector);
}

void ComputationManager::getFeatureDetector(int &featureDetector)
{
  featureDetector = m_featureDetector;
}

void ComputationManager::trainClassifier(QString className, std::vector<SamaelImage*> &images)
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

void ComputationManager::createVocabulary(int id, std::vector<cv::Mat> trainingDescriptors)
{
  for(int i = 0; i < trainingDescriptors.size(); i++)
  {
    m_bowTrainer->add(trainingDescriptors[i]);
  }

  m_classVocabularies[id] = m_bowTrainer->cluster();
}

void ComputationManager::classify(std::vector<SamaelImage*> &images, std::vector<QString> &out_classNames)
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
    }
  }

  out_classNames = std::vector<QString>();
}

void ComputationManager::SIFT(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::SiftFeatureDetector>(new cv::SiftFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new BFMatcher(NORM_L2, false));
  cv::Ptr<cv::DescriptorExtractor> extractor(new OpponentColorDescriptorExtractor(Ptr<DescriptorExtractor>(new cv::SiftDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  detector->detect(images, imageKeyPoints);//create/detect keypoints
  extractor->compute(images, imageKeyPoints, imageDescriptors);//create keypoint descriptors
}

void ComputationManager::SURF(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::SurfFeatureDetector>(new cv::SurfFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new BFMatcher(NORM_L2, false));
  cv::Ptr<cv::DescriptorExtractor> extractor(new OpponentColorDescriptorExtractor(Ptr<DescriptorExtractor>(new cv::SurfDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  detector->detect(images, imageKeyPoints);//create/detect keypoints
  extractor->compute(images, imageKeyPoints, imageDescriptors);//create keypoint descriptors
  

  //Mat img_1; //image
  //Mat img_2; //scene

  ////if( !img_1.data || !img_2.data )
  ////{ std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  ////Detect the keypoints using SURF Detector
  //int minHessian = 400;

  //SurfFeatureDetector detector( minHessian );

  //std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

  //detector.detect( img_1, keypoints_1 );
  //detector.detect( img_2, keypoints_2 );

  ////-- Draw keypoints
  //Mat img_keypoints_1; Mat img_keypoints_2;

  //drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  //drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

  ////-- Show detected (drawn) keypoints
  //imshow("Keypoints 1", img_keypoints_1 );
  //imshow("Keypoints 2", img_keypoints_2 );

  //waitKey(0);
  //
  //return 0;
}

void ComputationManager::MSER(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors)
{
  cv::Ptr<cv::FeatureDetector> detector = cv::Ptr<cv::MserFeatureDetector>(new cv::MserFeatureDetector());
  cv::Ptr<cv::DescriptorMatcher> matcher(new BFMatcher(NORM_L2, false));
  cv::Ptr<cv::DescriptorExtractor> extractor(new OpponentColorDescriptorExtractor(Ptr<DescriptorExtractor>(new cv::SiftDescriptorExtractor())));

  m_bowExtractor = new cv::BOWImgDescriptorExtractor(extractor, matcher);

  detector->detect(images, imageKeyPoints);//create/detect keypoints
  extractor->compute(images, imageKeyPoints, imageDescriptors);//create keypoint descriptors

	//Mat box = imread("C:\box.png",1);
 //   MSER ms;
 //   vector<vector<Point>> regions;
 //   ms(box, regions, Mat());
 //   for (int i = 0; i < regions.size(); i++)
 //   {
 //       ellipse(box, fitEllipse(regions[i]), Scalar(255));
 //   }
 //   imshow("mser", box);
 //   waitKey(0);

  //return 0;
}