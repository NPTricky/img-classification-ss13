#ifndef COMPUTATIONMANAGERBOW_H_
#define COMPUTATIONMANAGERBOW_H_

#include <QObject>

#include <vector>
#include <map>

#include "SamaelImage.h"
#include "Samael.h"
#include "Logger.h"

class ComputationManagerBOW : public QObject
{
public:

    static ComputationManagerBOW* getInstance(
        int clusterCount = 2,
        SAM::DetectorAdapter detectorAdapterType = SAM::DETECTOR_ADAPTER_NONE,
        SAM::Detector detectorType = SAM::DETECTOR_SIFT, 
        SAM::ExtractorAdapter extractorAdapterType = SAM::EXTRACTOR_ADAPTER_NONE, 
        SAM::Extractor extractorType = SAM::EXTRACTOR_SIFT,
        SAM::Matcher matcherType = SAM::MATCHER_FLANNBASED
    );
	
    Q_OBJECT
	
private:
  
  ComputationManagerBOW(
      int clusterCount, 
      SAM::DetectorAdapter detectorAdapterType,
      SAM::Detector detectorType,
      SAM::ExtractorAdapter extractorAdapterType,
      SAM::Extractor extractorType,
      SAM::Matcher matcherType
  );
  ~ComputationManagerBOW();

  //void detect(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors = nullptr);
  
  void computeKeyPoints(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints);
  void computeDescriptors(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors = nullptr);

  SAM::Detector m_detectorType;
  SAM::DetectorAdapter m_detectorAdapterType;
  SAM::Extractor m_extractorType;
  SAM::ExtractorAdapter m_extractorAdapterType;
  SAM::Matcher m_matcherType;

  cv::BOWKMeansTrainer *m_bowTrainer;
  cv::Ptr<cv::FeatureDetector> m_detector;
  cv::Ptr<cv::DescriptorMatcher> m_matcher;
  cv::Ptr<cv::DescriptorExtractor> m_extractor;
  cv::BOWImgDescriptorExtractor *m_bowExtractor;
  std::vector<std::string> m_classNames;//the class names
  
  cv::Mat m_vocabulary;//vocabulary for each class
  std::map<std::string, cv::Mat> m_histograms;//maps a histogram to each class
  std::map<std::string, CvSVM*> m_classifiers;//SVM classifiers of the classes

public slots:

  void getFeatureDetector(int &featureDetector);

  void setDetector(SAM::Detector detector = SAM::DETECTOR_SIFT, SAM::DetectorAdapter adapter = SAM::DETECTOR_ADAPTER_NONE);
  void setExtractor(SAM::Extractor extractor = SAM::EXTRACTOR_SIFT, SAM::ExtractorAdapter adapter = SAM::EXTRACTOR_ADAPTER_NONE);
  void setMatcher(SAM::Matcher matcher = SAM::MATCHER_FLANNBASED);
  void setTrainer(int clusterCount, double epsilon = 0.00001, int attempts = 3, int flag = cv::KMEANS_PP_CENTERS);

  void createVocabulary(std::map<std::string, std::vector<SamaelImage*>> &images);
  void trainClassifier(std::map<std::string, std::vector<SamaelImage*>> &images);//vector of images from one class
  void trainSVM();
  void classify(std::map<std::string, std::vector<SamaelImage*>> &images, std::vector<std::string> &out_classNames);//classifies a vector of images through returning the class names

private slots:
  void onDetectorExtractorChanged();
  void onMatcherExtractorChanged();

signals:
  void detectorChanged();
  void extractorChanged();
  void matcherChanged();
};

#endif