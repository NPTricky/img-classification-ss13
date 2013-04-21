#ifndef COMMANDLINELOGDESTINATION_H
#define COMMANDLINELOGDESTINATION_H

#include "LogDestination.h"

namespace QLog
{

    class CommandLineLogDestination : public LogDestination
    {
    public:
        CommandLineLogDestination();
        ~CommandLineLogDestination();

        virtual void write(const QString& message, Level level);
    };

} // namespace QLog

#endif // COMMANDLINELOGDESTINATION_H
