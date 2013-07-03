#include "stdafx.h"
#include "ComputationManagerBOW.h"
#include "ComputationParallel.h"

#include <QDebug>
#include <limits>
#include <QVector4d>
#include "Logger.h"

ComputationManagerBOW::ComputationManagerBOW(
    int clusterCount,
    SAM::FeatureAlgorithm algorithmType,
    SAM::DetectorAdapter detectorAdapterType,
    SAM::Detector detectorType,
    SAM::ExtractorAdapter extractorAdapterType,
    SAM::Extractor extractorType,
    SAM::Matcher matcherType
    )
{
    cv::initModule_features2d();
    cv::initModule_nonfree();
    cv::initModule_ml();

    connect(this,SIGNAL(detectorChanged()),this,SLOT(onDetectorExtractorChanged()));
    connect(this,SIGNAL(extractorChanged()),this,SLOT(onDetectorExtractorChanged()));
    connect(this,SIGNAL(matcherChanged()),this,SLOT(onMatcherExtractorChanged()));
    connect(this,SIGNAL(extractorChanged()),this,SLOT(onMatcherExtractorChanged()));

    m_algorithmType = SAM::FeatureAlgorithm(-1);
    m_detectorType = SAM::Detector(-1);
    m_detectorAdapterType = SAM::DetectorAdapter(-1);
    m_extractorType = SAM::Extractor(-1);
    m_extractorAdapterType = SAM::ExtractorAdapter(-1);
    m_matcherType = SAM::Matcher(-1);

    m_algorithm = nullptr;
    m_detector = nullptr;
    m_extractor = nullptr;
    m_matcher = nullptr;
    m_bowTrainer = nullptr;
    m_bowExtractor = nullptr;

    //setAlgorithm(algorithmType);
    setDetector(detectorType,detectorAdapterType);
    setExtractor(extractorType,extractorAdapterType);
    setMatcher(matcherType);
    setTrainer(clusterCount);

    QLOG_INFO() << "ComputationManagerBOW - Ready!";
}

ComputationManagerBOW::~ComputationManagerBOW()
{

}

ComputationManagerBOW* ComputationManagerBOW::getInstance(
    int clusterCount /*= 2*/,
    SAM::FeatureAlgorithm algorithmType /*= FEATURE_ALGORITHM_SIFT*/,
    SAM::DetectorAdapter detectorAdapterType /*= DETECTOR_ADAPTER_NONE*/, 
    SAM::Detector detectorType /*= DETECTOR_SIFT*/,
    SAM::ExtractorAdapter extractorAdapterType /*= EXTRACTOR_ADAPTER_NONE*/, 
    SAM::Extractor extractorType /*= EXTRACTOR_SIFT*/,
    SAM::Matcher matcherType /*= SAM::MATCHER_FLANNBASED*/
    )
{
  static ComputationManagerBOW instance(
      clusterCount,
      algorithmType,
      detectorAdapterType, 
      detectorType, 
      extractorAdapterType, 
      extractorType,
      matcherType
  );
  return &instance;
}

void ComputationManagerBOW::createVocabulary(std::map<std::string, std::vector<SamaelImage*>> &images)
{
  unsigned int stepCounter = 0;
  for(std::map<std::string, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    std::string className = it->first;
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

    computeKeyPoints(rawImageData, imageKeyPoints);
    computeDescriptors(rawImageData, imageKeyPoints, &imageDescriptors);

    for(int i = 0; i < imageDescriptors.size(); i++)
    {
      m_bowTrainer->add(imageDescriptors[i]);
    }

    stepCounter += classImages.size();
    printProgress("Create Descriptors", stepCounter, m_trainingImageNumber);
  }

  printProgress("Create Cluster (Vocabulary)", 0, 1);

  m_vocabulary = m_bowTrainer->cluster();

  printProgress("Create Cluster (Vocabulary)", 1, 1);
}

void ComputationManagerBOW::histogramCreation(std::map<std::string, std::vector<SamaelImage*>> &images)
{
  m_bowExtractor->setVocabulary(m_vocabulary);

  unsigned int stepCounter = 0;
  for(std::map<std::string, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++)
  {
    std::string className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<cv::Mat> rawImageData;
    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;

    imageKeyPoints.resize(classImages.size());

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }

    computeKeyPoints(rawImageData, imageKeyPoints);

    cv::Mat histogram;

    for(int j = 0; j < rawImageData.size(); j++)
    {
      m_bowExtractor->compute(rawImageData[j], imageKeyPoints[j], histogram);

      if(m_histograms.count(className) == 0)
      {
        m_histograms[className].create(0, histogram.cols, histogram.type());
      }
      m_histograms[className].push_back(histogram);

      printProgress("Create Histograms", ++stepCounter, m_trainingImageNumber);
    }
  }
}

void ComputationManagerBOW::trainSVM()
{
  unsigned int stepCounter = 0;
  for(int i = 0; i < m_classNames.size(); i++)
  {
    std::string className = m_classNames[i];

    cv::Mat samples(0, m_histograms[className].cols, m_histograms[className].type());
    cv::Mat labels(0, 1, CV_32FC1);

    samples.push_back(m_histograms[className]);
    cv::Mat classLabel = cv::Mat::ones(m_histograms[className].rows, 1, CV_32FC1);
    labels.push_back(classLabel);

    for(std::map<std::string, cv::Mat>::iterator hit = m_histograms.begin(); hit != m_histograms.end(); hit++)
    {
      std::string notClassName = hit->first;

      if(!className.compare(notClassName))
      {
        continue;
      }

      samples.push_back(m_histograms[notClassName]);
      classLabel = cv::Mat::zeros(m_histograms[notClassName].rows, 1, CV_32FC1);
      labels.push_back(classLabel);
    }

    cv::Mat samples_32f; 
    samples.convertTo(samples_32f, CV_32F);
    if(samples.rows == 0) 
    {
      continue; //phantom class?!
    }
    CvSVM *classifier = new CvSVM; 
    classifier->train(samples_32f, labels);
    m_classifiers[className] = classifier;

    printProgress("Train SVM", ++stepCounter, unsigned int(m_classNames.size()));
  }
}

void ComputationManagerBOW::classify(std::map<std::string, std::vector<SamaelImage*>> &images)
{
  int correctClassification = 0;
  int imageSize = 0;

  int matRow = 0;
  unsigned int stepCounter = 0;
  for(std::map<std::string, std::vector<SamaelImage*>>::iterator it = images.begin(); it != images.end(); it++, matRow++)
  {
    imageSize += int(it->second.size());

    std::string className = it->first;
    std::vector<SamaelImage*> classImages = it->second;

    std::vector<cv::Mat> rawImageData;
    std::vector<std::vector<cv::KeyPoint>> imageKeyPoints;

    imageKeyPoints.resize(classImages.size());

    for(int i = 0; i < classImages.size(); i++)
    {
      rawImageData.push_back(classImages[i]->getMat());
    }

    computeKeyPoints(rawImageData, imageKeyPoints);

    m_bowExtractor->setVocabulary(m_vocabulary);

    for(int j = 0; j < rawImageData.size(); j++)
    {
      cv::Mat histogram;
      m_bowExtractor->compute(rawImageData[j], imageKeyPoints[j], histogram);

      float minf = FLT_MAX; 
      std::string minClass;

      int matCol = 0;
      int winnerCol;
      for(std::map<std::string, CvSVM*>::iterator cit = m_classifiers.begin(); cit != m_classifiers.end(); cit++, matCol++)
      {
        float response = cit->second->predict(histogram, true);

        if(response < minf)
        {
          winnerCol = matCol;
          minf = response;
          minClass = cit->first;
        }
      }

      m_confusionMatrix.at<float>(winnerCol, matRow)++;

      if(!minClass.compare(className))
      {
        correctClassification++;
      }

      printProgress("Classify Images", ++stepCounter, m_classifyImageNumber);
    }
  }

  QLOG_INFO_NOCONTEXT() << float(correctClassification) / float(imageSize) * 100 << "% correct classification.\n";
}

void ComputationManagerBOW::doClassification()
{
  std::map<std::string, std::vector<SamaelImage*>> trainingImages, classifyImages;
  m_classNames.clear();

  emit getClassNames(m_classNames);

  emit getTrainingImages(trainingImages);
  emit getClassifyImages(classifyImages);

  m_trainingImageNumber = 0;
  for(std::map<std::string, std::vector<SamaelImage*>>::iterator it = trainingImages.begin(); it != trainingImages.end(); it++)
  {
    m_trainingImageNumber += it->second.size();
  }

  m_classifyImageNumber = 0;
  for(std::map<std::string, std::vector<SamaelImage*>>::iterator it = classifyImages.begin(); it != classifyImages.end(); it++)
  {
    m_classifyImageNumber += it->second.size();
  }

  m_confusionMatrix = cv::Mat::zeros(int(m_classNames.size()), int(m_classNames.size()), CV_32FC1);

  for(m_run = 0; m_run < TESTRUNS; m_run++)
  {
    createVocabulary(trainingImages);
    histogramCreation(trainingImages);
    trainSVM();
  
    classify(classifyImages);
  }

  m_confusionMatrix /= float(TESTRUNS);

  setConfusionMatrixHeaderData(m_classNames);
  displayMatrix(m_confusionMatrix);
}

void ComputationManagerBOW::printProgress(std::string stepName, unsigned int actually, unsigned int maximum)
{
  QLOG_TRACE_NOCONTEXT() << "Run: (" << m_run + 1 << "/" << TESTRUNS << ")" << stepName.c_str() << float(actually) / float(maximum) * 100.0f;
}

void ComputationManagerBOW::computeKeyPoints(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints)
{
    for(int i = 0; i < images.size(); i++)
    {
        m_detector->detect(images[i], out_imageKeyPoints[i]);
    }
    //tbb::parallel_for(tbb::blocked_range<size_t>(0,images.size()),ParallelComputeKeyPoints(m_detector,images,out_imageKeyPoints));
}

void ComputationManagerBOW::computeDescriptors(std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, std::vector<cv::Mat> *out_imageDescriptors /*= nullptr*/)
{
    if(out_imageDescriptors != nullptr)
    {
        for(int i = 0; i < images.size(); i++)
        {
            m_extractor->compute(images[i], imageKeyPoints[i], (*out_imageDescriptors)[i]);
        }
        //tbb::parallel_for(tbb::blocked_range<size_t>(0,images.size()),ParallelComputeDescriptors(m_extractor,images,imageKeyPoints,out_imageDescriptors));
    }
}

void ComputationManagerBOW::setDetector(SAM::Detector detector /*= SAM::DETECTOR_SIFT*/, SAM::DetectorAdapter adapter /*= SAM::DETECTOR_ADAPTER_NONE*/)
{
    if (detector == m_detectorType && adapter == m_detectorAdapterType)
        return;

    if (m_detector)
        delete m_detector;

    m_detector = cv::FeatureDetector::create(DetectorToText(adapter,detector));
    m_detectorType = detector;
    m_detectorAdapterType = adapter;

    emit detectorChanged();
}

void ComputationManagerBOW::setExtractor(SAM::Extractor extractor /*= SAM::EXTRACTOR_SIFT*/, SAM::ExtractorAdapter adapter /*= SAM::EXTRACTOR_ADAPTER_NONE*/)
{
    if (extractor == m_extractorType && adapter == m_extractorAdapterType)
        return;

    if (m_extractor)
        delete m_extractor;

    m_extractor = cv::DescriptorExtractor::create(ExtractorToText(adapter,extractor));
    m_extractorType = extractor;
    m_extractorAdapterType = adapter;

    emit extractorChanged();
}

void ComputationManagerBOW::setMatcher(SAM::Matcher matcher /*= SAM::MATCHER_FLANNBASED*/)
{
    if (matcher == m_matcherType)
        return;

    if (m_matcher)
        delete m_matcher;

    m_matcher = cv::DescriptorMatcher::create(MatcherToText(matcher));
    m_matcherType = matcher;

    emit matcherChanged();
}

void ComputationManagerBOW::setTrainer(
    int clusterCount,
    double epsilon /*= 0.00001*/, 
    int attempts /*= 3*/, 
    int flag /*= cv::KMEANS_PP_CENTERS*/
    )
{
    if (m_bowTrainer)
        delete m_bowTrainer;

    // CV_TERMCRIT_EPS: only the required precision (epsilon) does matter
    // (many algorithms limit the number of iterations anyway)
    cv::TermCriteria criteria = cv::TermCriteria(CV_TERMCRIT_EPS, 0, epsilon);
    m_bowTrainer = new cv::BOWKMeansTrainer(clusterCount, criteria, attempts, flag);
}

void ComputationManagerBOW::setAlgorithm(SAM::FeatureAlgorithm algorithm /*= SAM::FEATURE_ALGORITHM_SIFT*/)
{
    if (algorithm == m_algorithmType)
        return;

    if (m_algorithm)
        delete m_algorithm;

    m_algorithm = cv::Algorithm::create<cv::Feature2D>(AlgorithmToText(algorithm));
    m_algorithmType = algorithm;
}

void ComputationManagerBOW::onMatcherExtractorChanged()
{
    if (m_matcher && m_extractor)
        m_bowExtractor = new cv::BOWImgDescriptorExtractor(m_extractor, m_matcher);
}

void ComputationManagerBOW::onDetectorExtractorChanged()
{
    if (m_bowTrainer)
        m_bowTrainer->clear();//clears all previous vocabularies added to the trainer

    if(!m_vocabulary.empty())
        m_vocabulary.deallocate();//resets the vocabulary if the method changes
}

void ComputationManagerBOW::saveConfusionMatrix()
{
  cv::FileStorage file("confusionMatrix.xml", cv::FileStorage::WRITE);
  file << "Confusion_Matrix" << m_confusionMatrix;
  file.release();
}

void ComputationManagerBOW::loadConfusionMatrix()
{
  cv::FileStorage file("confusionMatrix.xml", cv::FileStorage::READ);
  file["Confusion_Matrix"] >> m_confusionMatrix;
  file.release();

  emit displayMatrix(m_confusionMatrix);
}
