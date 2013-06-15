#ifndef COMPUTATIONMANAGER_H_
#define COMPUTATIONMANAGER_H_

#include <vector>
#include <map>

#include <QObject>

#include "SamaelImage.h"

class ComputationManager : public QObject
{
public:

  enum Detector
  {
    DETECTOR_SIFT,
    DETECTOR_SURF,
    DETECTOR_MSER
  };

  static ComputationManager* getInstance(int clusterNumber, Detector featureDetector);

  Q_OBJECT

private:
  
  ComputationManager(int clusterNumber, Detector featureDetector);
  ~ComputationManager();

  void createVocabulary(int id, std::vector<cv::Mat> trainingDescriptors);//creates a vocabulary for each class

  //compute the descriptors and keypoints for the images
  void SIFT(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors);
  void SURF(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors);
  void MSER(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> &imageDescriptors);

  //void loadImg(QString imagePath);//file-loading shouldnt be part of computation manager
  
  //cv::KeyPoint *KeyPoint;

  //SamaelImage image;
  //SamaelImage temp;
  //SamaelImage grey;
  //IplImage* imgSurf;

  Detector m_featureDetector;

  cv::BOWKMeansTrainer *m_bowTrainer;
  cv::BOWImgDescriptorExtractor *m_bowExtractor;
  std::map<QString, int> m_classNameID;//maps the class name to a unique id
  std::vector<cv::Mat> m_classVocabularies;//vocabulary of each class

public slots:

  void setFeatureDetector(int featureDetector);
  void getFeatureDetector(int &featureDetector); 
  void trainClassifier(QString className, std::vector<SamaelImage*> &images);//vector of images from one class
  void classify(std::vector<SamaelImage*> &images, std::vector<QString> &out_classNames);//classifies a vector of images through returning the class names
};

#endif