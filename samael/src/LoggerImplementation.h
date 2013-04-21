#ifndef LOGGERIMPLEMENTATION_H
#define LOGGERIMPLEMENTATION_H

#include "Factory.h"
#include "LogDestination.h"

namespace QLog
{

    typedef std::vector<std::unique_ptr<LogDestination>> DestinationGroup;
    typedef std::map<QString,DestinationGroup> DestinationMap;

    class LoggerImplementation
    {
    public:
        LoggerImplementation()
            : myLoggerLevel(InfoLevel)
            , myLoggerDestinationGroup("Default")
        {

        }

        QMutex myMutex;

        Level myLoggerLevel;
        QString myLoggerDestinationGroup;

        DestinationMap myDestinationMap;
        Factory<QString,LogDestination> myFactory;
    };

} // namespace QLog

#endif // LOGGERIMPLEMENTATION_H
