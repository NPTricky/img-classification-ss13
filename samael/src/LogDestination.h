#ifndef LOGDESTINATION_H
#define LOGDESTINATION_H

#include "LogLevel.h"

namespace QLog
{

    class LogDestination
    {
    public:
        virtual ~LogDestination(){};
        virtual void write(const QString& message, Level level) = 0;
    };

} // namespace QLog

#endif // LOGDESTINATION_H
