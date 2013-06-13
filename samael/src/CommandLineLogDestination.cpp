#include "stdafx.h"
#include "CommandLineLogDestination.h"

#if defined(Q_OS_WIN)
#include <Windows.h>
#include <iostream>
#endif

#if defined(Q_OS_UNIX)
#include <cstdio>
#endif

namespace QLog
{

    CommandLineLogDestination::CommandLineLogDestination()
        : LogDestination()
    {

    }

    CommandLineLogDestination::~CommandLineLogDestination()
    {

    }

    #if defined(Q_OS_WIN)
    void CommandLineLogDestination::write(const QString& message, Level level)
    {
        // debugger output
        OutputDebugStringW(reinterpret_cast<const WCHAR*>(message.utf16()));
        OutputDebugStringW(L"\n");

        // command line output
        if (level >= ErrorLevel)
            std::cout << message.toStdString() << std::endl;
    }
    #endif // defined(Q_OS_WIN)

    #if defined(Q_OS_UNIX)
    void CommandLineLogDestination::write(const QString& message, Level level)
    {
        fprintf(stderr, "%s\n", qPrintable(message));
        fflush(stderr);
    }
    #endif // defined(Q_OS_UNIX)

} // namespace QLog