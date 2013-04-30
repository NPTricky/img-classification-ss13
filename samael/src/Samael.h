#ifndef ITEM_H
#define ITEM_H

#include <QString>

namespace Samael
{

    enum Set
    {
        TestSet = 0
        , TrainingSet
    };

    static QString SetToText(Set set)
    {
        switch(set)
        {
        case TestSet:
            return QString("Test Set");
        case TrainingSet:
            return QString("Training Set");
        default:
            {
                Q_ASSERT_X(false,"Bad Set","static QString Item::SetToText(Set)");
                return QString("Test Set");
            }
        }
    }

} // namespace Item

#endif // ITEM_H
