#ifndef SAMAEL_H
#define SAMAEL_H

#include <QString>
#include "Logger.h"

namespace SAM
{

    enum DetectorAdapter
    {
        DETECTOR_ADAPTER_NONE = -1,
        DETECTOR_ADAPTER_GRID,
        DETECTOR_ADAPTER_PYRAMID
    };

    enum Detector
    {
        DETECTOR_NONE = -1,
        DETECTOR_SIFT,
        DETECTOR_SURF,
        DETECTOR_MSER,
        DETECTOR_ORB
    };

    static std::string DetectorToText(DetectorAdapter adapter, Detector detector)
    {
        std::string result;

        switch(adapter)
        {
        case DETECTOR_ADAPTER_NONE:
            break;
        case DETECTOR_ADAPTER_GRID:
            result.append("Grid");
            break;
        case DETECTOR_ADAPTER_PYRAMID:
            result.append("Pyramid");
            break;
        default:
            {
                QLOG_WARN() << "DetectorToText() Fallback ( DETECTOR_ADAPTER_NONE ) - Bad DetectorAdapter";
                break;
            }
        }

        switch(detector)
        {
        case DETECTOR_SIFT:
            result.append("SIFT");
            break;
        case DETECTOR_SURF:
            result.append("SURF");
            break;
        case DETECTOR_ORB:
            result.append("ORB");
            break;
        case DETECTOR_MSER:
            result.append("MSER");
            break;
        default:
            {
                std::string fallback = "SIFT";
                QLOG_WARN() << "DetectorToText() Fallback (" << fallback.c_str() << ") - Bad Detector";
                result.append(fallback);
                break;
            }
        }

        return result;
    }

    enum ExtractorAdapter
    {
        EXTRACTOR_ADAPTER_NONE = -1,
        EXTRACTOR_ADAPTER_OPPONENT
    };

    enum Extractor
    {
        EXTRACTOR_NONE = -1,
        EXTRACTOR_SIFT,
        EXTRACTOR_SURF,
        EXTRACTOR_ORB
    };

    static std::string ExtractorToText(ExtractorAdapter adapter, Extractor extractor)
    {
        std::string result;

        switch(adapter)
        {
        case EXTRACTOR_ADAPTER_NONE:
            break;
        case EXTRACTOR_ADAPTER_OPPONENT:
            result.append("Opponent");
            break;
        default:
            {
                QLOG_WARN() << "ExtractorToText() Fallback ( EXTRACTOR_ADAPTER_NONE ) - Bad ExtractorAdapter";
                break;
            }
        }

        switch(extractor)
        {
        case EXTRACTOR_SIFT:
            result.append("SIFT");
            break;
        case EXTRACTOR_SURF:
            result.append("SURF");
            break;
        case EXTRACTOR_ORB:
            result.append("ORB");
            break;
        default:
            {
                std::string fallback = "SIFT";
                QLOG_WARN() << "ExtractorToText() Fallback (" << fallback.c_str() << ") - Bad Extractor";
                result.append(fallback);
                break;
            }
        }

        return result;
    }

    enum Matcher
    {
        MATCHER_BRUTEFORCE_L1,
        MATCHER_BRUTEFORCE_L2,
        MATCHER_BRUTEFORCE_HAMMING_1,
        MATCHER_BRUTEFORCE_HAMMING_2,
        MATCHER_FLANNBASED
    };

    static std::string MatcherToText(Matcher matcher)
    {
        switch(matcher)
        {
        case MATCHER_BRUTEFORCE_L1:
            return std::string("BruteForce-L1");
        case MATCHER_BRUTEFORCE_L2:
            return std::string("BruteForce-L2");
        case MATCHER_BRUTEFORCE_HAMMING_1:
            return std::string("BruteForce-Hamming");
        case MATCHER_BRUTEFORCE_HAMMING_2:
            return std::string("BruteForce-Hamming(2)");
        case MATCHER_FLANNBASED:
            return std::string("FlannBased");
        default:
            {
                std::string fallback = "FlannBased";
                QLOG_WARN() << "MatcherToText Fallback (" << fallback.c_str() << ") - Bad Matcher";
                return fallback;
            }
        }
    }

    enum Set
    {
        SET_TEST,
        SET_TRAINING
    };

    static QString SetToText(Set set)
    {
        switch(set)
        {
        case SET_TEST:
            return QString("Test");
        case SET_TRAINING:
            return QString("Training");
        default:
            {
                QString fallback = "Test";
                QLOG_WARN() << "SetToText Fallback (" << fallback << ") - Bad Set";
                return fallback;
            }
        }
    }

} // namespace Item

#endif // SAMAEL_H
