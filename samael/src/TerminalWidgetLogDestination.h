#ifndef TERMINALWIDGETLOGDESTINATION_H
#define TERMINALWIDGETLOGDESTINATION_H

#include "LogDestination.h"
#include "TerminalWidget.h"

namespace QLog
{

    class TerminalWidgetLogDestination : public LogDestination
    {
    public:
        TerminalWidgetLogDestination(TerminalWidget* destination = nullptr);
        ~TerminalWidgetLogDestination();

        virtual void write(const QString& message, Level level);

    private:
        TerminalWidget* myTerminalWidget;
    };

} // namespace QLog

#endif // TERMINALWIDGETLOGDESTINATION_H
