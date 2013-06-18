#ifndef COMPUTATIONMANAGERBOW_H_
#define COMPUTATIONMANAGERBOW_H_

#include <vector>
#include <map>

#include <QObject>

#include "SamaelImage.h"

class ComputationManagerBOW : public QObject
{
public:

  enum Detector
  {
    DETECTOR_SIFT,
    DETECTOR_SURF,
    DETECTOR_MSER
  };

  static ComputationManagerBOW* getInstance(int clusterNumber, Detector featureDetector);

  Q_OBJECT

private:
  
  ComputationManagerBOW(int clusterNumber, Detector featureDetector);
  ~ComputationManagerBOW();

  //compute the descriptors and keypoints for the images
  void SIFT(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors = nullptr);
  void SURF(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors = nullptr);
  void MSER(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors = nullptr);

  Detector m_featureDetector;

  cv::BOWKMeansTrainer *m_bowTrainer;
  cv::BOWImgDescriptorExtractor *m_bowExtractor;
  std::vector<QString> m_classNames;//the class names
  
  cv::Mat m_vocabulary;//vocabulary for each class
  std::map<QString, cv::Mat> m_histograms;//maps a histogram to each class
  std::map<QString, CvSVM> m_classifiers;//SVM classifiers of the classes

public slots:

  void setFeatureDetector(int featureDetector);
  void getFeatureDetector(int &featureDetector);
  void createVocabulary(std::map<QString, std::vector<SamaelImage*>> &images);
  void trainClassifier(std::map<QString, std::vector<SamaelImage*>> &images);//vector of images from one class
  void trainSVM();
  void classify(std::map<QString, std::vector<SamaelImage*>> &images, std::vector<QString> &out_classNames);//classifies a vector of images through returning the class names
};

#endif