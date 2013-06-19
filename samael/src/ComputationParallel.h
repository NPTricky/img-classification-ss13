#ifndef COMPUTATIONPARALLEL_H
#define COMPUTATIONPARALLEL_H

#include <tbb/tbb.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

//for(int i = 0; i < images.size(); i++)
//{
//    m_detector->detect(images[i], out_imageKeyPoints[i]);
//}
class ParallelComputeKeyPoints
{
public:
    ParallelComputeKeyPoints(
        cv::Ptr<cv::FeatureDetector> detector, 
        std::vector<cv::Mat> &images, 
        std::vector<std::vector<cv::KeyPoint>> &out_imageKeyPoints
        )
        : m_detector(detector)
        , m_images(images)
        , m_out_imageKeyPoints(out_imageKeyPoints)
    {}

    void operator()(const tbb::blocked_range<size_t>& range) const
    {
        for (size_t i = range.begin(); i != range.end(); i++)
            m_detector->detect(m_images[i], m_out_imageKeyPoints[i]);
    }
    
private:
    cv::Ptr<cv::FeatureDetector> m_detector;
    std::vector<cv::Mat> const &m_images;
    std::vector<std::vector<cv::KeyPoint>> &m_out_imageKeyPoints;
};

//for(int i = 0; i < images.size(); i++)
//{
//    m_extractor->compute(images[i], imageKeyPoints[i], (*out_imageDescriptors)[i]);
//}
class ParallelComputeDescriptors
{
public:
    ParallelComputeDescriptors(
        cv::Ptr<cv::DescriptorExtractor> extractor,
        std::vector<cv::Mat> &images, 
        std::vector<std::vector<cv::KeyPoint>> &imageKeyPoints, 
        std::vector<cv::Mat> *out_imageDescriptors
        )
        : m_extractor(extractor)
        , m_images(images)
        , m_imageKeyPoints(imageKeyPoints)
        , m_out_imageDescriptors(out_imageDescriptors)
    {}

    void operator()(const tbb::blocked_range<size_t>& range) const
    {
        for (size_t i = range.begin(); i != range.end(); i++)
            m_extractor->compute(m_images[i], m_imageKeyPoints[i], (*m_out_imageDescriptors)[i]);
    }

private:
    cv::Ptr<cv::DescriptorExtractor> m_extractor;
    std::vector<cv::Mat> const &m_images;
    std::vector<std::vector<cv::KeyPoint>> &m_imageKeyPoints;
    std::vector<cv::Mat> *m_out_imageDescriptors;
};

#endif // COMPUTATIONPARALLEL_H
