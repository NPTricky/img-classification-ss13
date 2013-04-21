#ifndef LOGDESTINATIONCREATOR_H
#define LOGDESTINATIONCREATOR_H

#include "DerivedCreator.h"
#include "LogDestination.h"
#include "FileLogDestination.h"
#include "CommandLineLogDestination.h"

namespace QLog
{

    template <class DerivedType>
    class LogDestinationCreator : public DerivedCreator<LogDestination, DerivedType> {};

    template <>
    class LogDestinationCreator<FileLogDestination> : public DerivedCreator<LogDestination, FileLogDestination>
    {
        std::unique_ptr<LogDestination> Create(const QString& filename) 
        {
            return std::unique_ptr<LogDestination>(new FileLogDestination(filename));
        }
    };

} // namespace QLog

#endif // LOGDESTINATIONCREATOR_H
