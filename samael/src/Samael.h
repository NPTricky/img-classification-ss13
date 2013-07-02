#ifndef SAMAEL_H
#define SAMAEL_H

#include <QString>
#include "Logger.h"

namespace SAM
{
    enum FeatureAlgorithm
    {
        FEATURE_ALGORITHM_NONE = -1,
        FEATURE_ALGORITHM_SIFT,
        FEATURE_ALGORITHM_SURF,
        FEATURE_ALGORITHM_BRISK,
        FEATURE_ALGORITHM_FAST,
        FEATURE_ALGORITHM_MSER,
        FEATURE_ALGORITHM_ORB,
        FEATURE_ALGORITHM_FREAK,
        FEATURE_ALGORITHM_GFTT,
        FEATURE_ALGORITHM_HARRIS
    };

    static std::string AlgorithmToText(FeatureAlgorithm algorithm)
    {
        std::string result;

        switch(algorithm)
        {
        case FEATURE_ALGORITHM_SIFT:
            result.append("Feature2D.SIFT");
            break;
        case FEATURE_ALGORITHM_SURF:
            result.append("Feature2D.SURF");
            break;
        case FEATURE_ALGORITHM_BRISK:
            result.append("Feature2D.BRISK");
            break;
        case FEATURE_ALGORITHM_FAST:
            result.append("Feature2D.FAST");
            break;
        case FEATURE_ALGORITHM_MSER:
            result.append("Feature2D.MSER");
            break;
        case FEATURE_ALGORITHM_ORB:
            result.append("Feature2D.ORB");
            break;
        case FEATURE_ALGORITHM_FREAK:
            result.append("Feature2D.FREAK");
            break;
        case FEATURE_ALGORITHM_GFTT:
            result.append("Feature2D.GFTT");
            break;
        case FEATURE_ALGORITHM_HARRIS:
            result.append("Feature2D.HARRIS");
            break;
        default:
            {
                QLOG_WARN_NOCONTEXT() << "AlgorithmToText() FEATURE_ALGORITHM_NONE";
                break;
            }
        }

        return result;
    }

    enum DetectorAdapter
    {
        DETECTOR_ADAPTER_NONE = -1,
        DETECTOR_ADAPTER_GRID,
        DETECTOR_ADAPTER_PYRAMID,
        DETECTOR_ADAPTER_DYNAMIC
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
        case DETECTOR_ADAPTER_GRID:
            result.append("Grid");
            break;
        case DETECTOR_ADAPTER_PYRAMID:
            result.append("Pyramid");
            break;
        case DETECTOR_ADAPTER_DYNAMIC:
            result.append("Dynamic");
            break;
        default:
            {
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
                QLOG_WARN_NOCONTEXT() << "DetectorToText() DETECTOR_NONE";
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
        case EXTRACTOR_ADAPTER_OPPONENT:
            result.append("Opponent");
            break;
        default:
            {
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
                QLOG_WARN_NOCONTEXT() << "ExtractorToText() EXTRACTOR_NONE";
                break;
            }
        }

        return result;
    }

    enum Matcher
    {
        MATCHER_NONE = -1,
        MATCHER_BRUTEFORCE_L1,
        MATCHER_BRUTEFORCE_L2,
        MATCHER_BRUTEFORCE_HAMMING_1,
        MATCHER_BRUTEFORCE_HAMMING_2,
        MATCHER_FLANNBASED
    };

    static std::string MatcherToText(Matcher matcher)
    {
        std::string result;

        switch(matcher)
        {
        case MATCHER_BRUTEFORCE_L1:
            result.append("BruteForce-L1");
            break;
        case MATCHER_BRUTEFORCE_L2:
            result.append("BruteForce-L2");
            break;
        case MATCHER_BRUTEFORCE_HAMMING_1:
            result.append("BruteForce-Hamming");
            break;
        case MATCHER_BRUTEFORCE_HAMMING_2:
            result.append("BruteForce-Hamming(2)");
            break;
        case MATCHER_FLANNBASED:
            result.append("FlannBased");
            break;
        default:
            {
                QLOG_WARN_NOCONTEXT() << "MatcherToText() MATCHER_NONE";
                break;
            }
        }

        return result;
    }

    enum Set
    {
        SET_NONE = -1,
        SET_TEST,
        SET_TRAINING
    };

    static QString SetToText(Set set)
    {
        std::string result;

        switch(set)
        {
        case SET_TEST:
            result.append("Test");
            break;
        case SET_TRAINING:
            result.append("Training");
            break;
        default:
            {
                QLOG_INFO_NOCONTEXT() << "SetToText() SET_NONE";
                break;
            }
        }
    }

} // namespace Item

#endif // SAMAEL_H
