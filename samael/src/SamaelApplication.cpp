#include "stdafx.h"
#include "SamaelApplication.h"
#include "LogDestinationCreator.h"
#include "CommandLineLogDestination.h"
#include "FileLogDestination.h"
#include "TerminalWidgetLogDestination.h"
#include "Logger.h"

static const bool LOGGER_WELCOME = false;
static const bool LOGGER_FUNCTIONALITY_CHECK = false; 

SamaelApplication::SamaelApplication(int& argc, char** argv)
    : QApplication(argc, argv)
    , m_MainWindow(new SamaelMainWindow(nullptr))
    , m_Logger(QLog::Logger::Instance())
{

}

SamaelApplication::~SamaelApplication()
{
    delete m_MainWindow;
}

void LoggerMessageHandler(QtMsgType type, const QMessageLogContext & context, const QString & message)
{
        switch (type)
        {
        case QtDebugMsg:
            QLOG_DEBUG_NOCONTEXT() << QString("%1 [%2]").arg(context.file).arg(context.line, 4, 10, QChar('0')).toStdString().c_str() << "QT:" << message;
            break;
        case QtWarningMsg:
            QLOG_WARN_NOCONTEXT() << QString("%1 [%2]").arg(context.file).arg(context.line, 4, 10, QChar('0')).toStdString().c_str() << "QT:" << message;
            break;
        case QtCriticalMsg:
            QLOG_ERROR_NOCONTEXT() << QString("%1 [%2]").arg(context.file).arg(context.line, 4, 10, QChar('0')).toStdString().c_str() << "QT:" << message;
            break;
        case QtFatalMsg:
            QLOG_FATAL_NOCONTEXT() << QString("%1 [%2]").arg(context.file).arg(context.line, 4, 10, QChar('0')).toStdString().c_str() << "QT:" << message;
            break;
        }
}

void SamaelApplication::initialize()
{
    //qInstallMessageHandler(LoggerMessageHandler);

    // fill the logger creator registry with the available LogDestinationCreators
    m_Logger.registerCreator("CommandLineLogDestination",new QLog::LogDestinationCreator<QLog::CommandLineLogDestination>);
    m_Logger.registerCreator("FileLogDestination",new QLog::LogDestinationCreator<QLog::FileLogDestination>);
    m_Logger.registerCreator("TerminalWidgetLogDestination",new QLog::LogDestinationCreator<QLog::TerminalWidgetLogDestination>);
    
    // create the first log group (no group name given = default name = "Default")
    m_Logger.create("CommandLineLogDestination");
    m_Logger.create("FileLogDestination");

    // advanced creation of a terminal widget to receive the output
    TerminalWidget* terminal = new TerminalWidget(m_MainWindow);
    m_Logger.getDestinationGroup()->push_back(std::unique_ptr<QLog::TerminalWidgetLogDestination>(new QLog::TerminalWidgetLogDestination(terminal)));

    m_Logger.setLogLevel(QLog::TraceLevel);
    
    if (LOGGER_WELCOME)
    {
        QLOG_INFO() << "As-Salamu Alaykum!";
        QLOG_INFO() << "Project Samael - Excelling in Image Classification since 2013.";
        QLOG_INFO() << "Copyright: Sarah Pauksch, Christoph Laemmerhirt, Tim Benedict Jagla" << "\n";
    }

    if (LOGGER_FUNCTIONALITY_CHECK)
    {
        // warm up the logger...
        QLOG_INFO() << "Application Properties...";
        QLOG_INFO() << "Q_VERSION:" << qVersion();
        QLOG_INFO() << "Application Directory Path:" << GetApp()->applicationDirPath();
        QLOG_INFO() << "Application Name:          " << GetApp()->applicationName();
        QLOG_INFO() << "Application Version:       " << GetApp()->applicationVersion();
        QLOG_INFO() << "Application Process ID:    " << GetApp()->applicationPid() << "\n";

        QLOG_INFO()  << "Check Logger Functionality...";
        QLOG_INFO()  << "Current Logger Level:" << QLog::LevelToText(m_Logger.getLogLevel());
        QLOG_INFO()  << "Current Logger Group:" << m_Logger.getDestinationGroupName();
        QLOG_TRACE() << "LevelToText(0) >" << QLog::LevelToText(QLog::TraceLevel) << "| ID:" << static_cast<int>(QLog::TraceLevel);
        QLOG_DEBUG() << "LevelToText(1) >" << QLog::LevelToText(QLog::DebugLevel) << "| ID:" << static_cast<int>(QLog::DebugLevel);
        QLOG_INFO()  << "LevelToText(2) >" << QLog::LevelToText(QLog::InfoLevel) << " | ID:" << static_cast<int>(QLog::InfoLevel);
        QLOG_WARN()  << "LevelToText(3) >" << QLog::LevelToText(QLog::WarnLevel) << " | ID:" << static_cast<int>(QLog::WarnLevel);
        QLOG_ERROR() << "LevelToText(4) >" << QLog::LevelToText(QLog::ErrorLevel) << "| ID:" << static_cast<int>(QLog::ErrorLevel);
        QLOG_FATAL() << "LevelToText(5) >" << QLog::LevelToText(QLog::FatalLevel) << "| ID:" << static_cast<int>(QLog::FatalLevel);
        QLOG_INFO()  << "Check Done!" << "\n";
    }

    QLOG_INFO() << "GUI Initialization...";
    m_MainWindow->initialize(terminal);
    QLOG_INFO() << "GUI Initialization Done! Awaiting Input..." << "\n";
    m_MainWindow->show();
}
