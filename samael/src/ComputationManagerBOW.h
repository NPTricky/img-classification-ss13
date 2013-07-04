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
        SAM::FeatureAlgorithm algorithmType = SAM::FEATURE_ALGORITHM_SIFT,
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
      SAM::FeatureAlgorithm algorithmType,
      SAM::DetectorAdapter detectorAdapterType,
      SAM::Detector detectorType,
      SAM::ExtractorAdapter extractorAdapterType,
      SAM::Extractor extractorType,
      SAM::Matcher matcherType
  );

  ~ComputationManagerBOW();

  void computeKeyPoints(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints);
  void computeDescriptors(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors = nullptr);
  
  void createVocabulary(std::map<std::string, std::vector<SamaelImage*>> &images);
  void histogramCreation(std::map<std::string, std::vector<SamaelImage*>> &images);//vector of images from one class
  void trainSVM();
  void classify(std::map<std::string, std::vector<SamaelImage*>> &images);//classifies a vector of images through returning the class names

  void printProgress(std::string stepName, unsigned int actually, unsigned int maximum);

  SAM::FeatureAlgorithm m_algorithmType;
  SAM::Detector m_detectorType;
  SAM::DetectorAdapter m_detectorAdapterType;
  SAM::Extractor m_extractorType;
  SAM::ExtractorAdapter m_extractorAdapterType;
  SAM::Matcher m_matcherType;

  cv::Ptr<cv::Feature2D> m_algorithm;
  cv::Ptr<cv::BOWKMeansTrainer> m_bowTrainer;
  cv::Ptr<cv::FeatureDetector> m_detector;
  cv::Ptr<cv::DescriptorMatcher> m_matcher;
  cv::Ptr<cv::DescriptorExtractor> m_extractor;
  cv::Ptr<cv::BOWImgDescriptorExtractor> m_bowExtractor;
  std::vector<std::string> m_classNames;//the class names
  
  cv::Mat m_vocabulary;//vocabulary for each class
  std::map<std::string, cv::Mat> m_histograms;//maps a histogram to each class
  std::map<std::string, CvSVM*> m_classifiers;//SVM classifiers of the classes

  cv::Mat m_confusionMatrix;//confusion Matrix of all Classes

  unsigned int m_trainingImageNumber;//number of training images
  unsigned int m_classifyImageNumber;//number of classifying images
  unsigned int m_run;//actual run of the test
  static const unsigned int TESTRUNS = 20;//maximum number of runs per test

public slots:

  void setAlgorithm(SAM::FeatureAlgorithm algorithm = SAM::FEATURE_ALGORITHM_SIFT);
  void setDetector(SAM::Detector detector = SAM::DETECTOR_SIFT, SAM::DetectorAdapter adapter = SAM::DETECTOR_ADAPTER_NONE);
  void setExtractor(SAM::Extractor extractor = SAM::EXTRACTOR_SIFT, SAM::ExtractorAdapter adapter = SAM::EXTRACTOR_ADAPTER_NONE);
  void setMatcher(SAM::Matcher matcher = SAM::MATCHER_FLANNBASED);
  void setTrainer(int clusterCount, double epsilon = 0.00001, int attempts = 3, int flag = cv::KMEANS_PP_CENTERS);

  void doClassification();

  void saveConfusionMatrix();
  void loadConfusionMatrix();

private slots:
  void onDetectorExtractorChanged();
  void onMatcherExtractorChanged();

signals:

  void getClassNames(std::vector<std::string> &out_classNames);
  void getTrainingImages(std::map<std::string, std::vector<SamaelImage*>> &out_images);
  void getClassifyImages(std::map<std::string, std::vector<SamaelImage*>> &out_images);

  void detectorChanged();
  void extractorChanged();
  void matcherChanged();
  void displayMatrix(cv::Mat &matrix);
  void setConfusionMatrixHeaderData(std::vector<std::string> &classNames);
};

#endif