#ifndef LOGDESTINATIONCREATOR_H
#define LOGDESTINATIONCREATOR_H

#include "DerivedCreator.h"
#include "LogDestination.h"
#include "FileLogDestination.h"
#include "CommandLineLogDestination.h"
#include "TerminalWidgetLogDestination.h"
#include "TerminalWidget.h"

namespace QLog
{

    template <class DerivedType>
    class LogDestinationCreator : public DerivedCreator<LogDestination, DerivedType>
    {
        template <typename T>
        std::unique_ptr<LogDestination> Create(T& argument) 
        {
            return std::unique_ptr<LogDestination>(new DerivedType(argument));
        }
    };

} // namespace QLog

#endif // LOGDESTINATIONCREATOR_H
