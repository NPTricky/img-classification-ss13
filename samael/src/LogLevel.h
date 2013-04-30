#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include <QString>

namespace QLog
{

    enum Level
    {
        TraceLevel = 0
        , DebugLevel
        , InfoLevel
        , WarnLevel
        , ErrorLevel
        , FatalLevel
    };

    static QString LevelToText(Level level)
    {
        switch(level)
        {
        case TraceLevel:
            return QString("TRACE");
        case DebugLevel:
            return QString("DEBUG");
        case InfoLevel:
            return QString("INFO");
        case WarnLevel:
            return QString("WARN");
        case ErrorLevel:
            return QString("ERROR");
        case FatalLevel:
            return QString("FATAL");
        default:
            {
                Q_ASSERT_X(false,"Bad Log Level","static QString QLog::LevelToText(Level)");
                return QString("INFO");
            }
        }
    }

} // namespace QLog

#endif // LOGLEVEL_H
