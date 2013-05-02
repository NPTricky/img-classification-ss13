#ifndef LOGGER_H
#define LOGGER_H

#include "Creator.h"
#include "LogLevel.h"
#include "LoggerImplementation.h"

namespace QLog
{

    class LogDestination;

    class Logger
    {
    public:
        static Logger& Instance()
        {
            static Logger myInstance;
            return myInstance;
        }

        void registerCreator(const QString& key, Creator<LogDestination>* creator);
        void create(const QString& key, const QString& groupName = "Default");

        void setLogLevel(Level level);
        Level Logger::getLogLevel();

        void setDestinationGroupName(const QString& groupName = "Default");
        QString Logger::getDestinationGroupName();

        DestinationGroup* getDestinationGroup(const QString& groupName = "Default"); ///< hack until variadic templates...

        class Helper
        {
        public:
            explicit Helper(Level level) 
                : myMessageLevel(level)
                , myDebug(&myStringBuffer)
            {

            }

            ~Helper();

            QDebug& stream();

        private:
            /// \brief creates and formats a log message with contextual information
            ///
            /// \return Nothing
            /// \sa Logger::LoggerImplementation,#pImpl,Logger::write(const QString& message, Level level, const QString& groupName)
            void write();

            Level myMessageLevel;
            QString myStringBuffer;
            QDebug myDebug;
        };

    private:
        Logger(); ///< private constructor
        ~Logger(); ///< private destructor
        Logger(const Logger&); ///< private copy constructor
        Logger& operator=(const Logger&); ///< private assignment operator

        /// \brief iterates over specific LogDestinations to pass the log message
        ///
        /// \param[in] message a formatted message for direct logging
        /// \param[in] level log relevance of the message
        /// \param[in] groupName name of the DestinationGroup to log into
        /// \return Nothing
        /// \code{.cpp}
        /// 
        /// \endcode
        /// \sa Logger::LoggerImplementation,#pImpl,DestinationGroup
        void write(const QString& message, Level level, const QString& groupName);

        std::unique_ptr<LoggerImplementation> pImpl;
        static Logger myInstance;
    };

} // namespace QLog

#define QLOG_TRACE() \
    if( QLog::Logger::Instance().getLogLevel() > QLog::TraceLevel ){} \
    else  QLog::Logger::Helper(QLog::TraceLevel).stream() << QString("%1 [%2]").arg(__FILE__).arg(__LINE__, 4, 10, QChar('0')).toStdString().c_str()
#define QLOG_DEBUG() \
    if( QLog::Logger::Instance().getLogLevel() > QLog::DebugLevel ){} \
    else QLog::Logger::Helper(QLog::DebugLevel).stream() << QString("%1 [%2]").arg(__FILE__).arg(__LINE__, 4, 10, QChar('0')).toStdString().c_str()
#define QLOG_INFO()  \
    if( QLog::Logger::Instance().getLogLevel() > QLog::InfoLevel ){} \
    else QLog::Logger::Helper(QLog::InfoLevel).stream() << QString("%1 [%2]").arg(__FILE__).arg(__LINE__, 4, 10, QChar('0')).toStdString().c_str()
#define QLOG_WARN()  \
    if( QLog::Logger::Instance().getLogLevel() > QLog::WarnLevel ){} \
    else QLog::Logger::Helper(QLog::WarnLevel).stream() << QString("%1 [%2]").arg(__FILE__).arg(__LINE__, 4, 10, QChar('0')).toStdString().c_str()
#define QLOG_ERROR() \
    if( QLog::Logger::Instance().getLogLevel() > QLog::ErrorLevel ){} \
    else QLog::Logger::Helper(QLog::ErrorLevel).stream() << QString("%1 [%2]").arg(__FILE__).arg(__LINE__, 4, 10, QChar('0')).toStdString().c_str()
#define QLOG_FATAL() \
    QLog::Logger::Helper(QLog::FatalLevel).stream() << QString("%1 [%2]").arg(__FILE__).arg(__LINE__, 4, 10, QChar('0')).toStdString().c_str()

#endif // LOGGER_H
