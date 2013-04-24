#include "stdafx.h"
#include "TerminalWidgetLogDestination.h"

namespace QLog
{

    TerminalWidgetLogDestination::TerminalWidgetLogDestination(TerminalWidget* destination)
        : LogDestination()
        , myTerminalWidget(destination)
    {

    }

    TerminalWidgetLogDestination::~TerminalWidgetLogDestination()
    {

    }

    void QLog::TerminalWidgetLogDestination::write(const QString& message, Level level)
    {
        myTerminalWidget->log(message);
    }

} // namespace QLog