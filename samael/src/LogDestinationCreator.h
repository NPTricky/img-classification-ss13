#ifndef LOGDESTINATIONCREATOR_H
#define LOGDESTINATIONCREATOR_H

#include "DerivedCreator.h"
#include "LogDestination.h"

namespace QLog
{

    template <class DerivedType>
    class LogDestinationCreator : public DerivedCreator<LogDestination, DerivedType> {};

} // namespace QLog

#endif // LOGDESTINATIONCREATOR_H
