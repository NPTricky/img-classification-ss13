#include "stdafx.h"
#include "Logger.h"
#include "Factory.h"
#include "LoggerImplementation.h"

namespace QLog
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors & Destructor
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Logger::Logger()
        : pImpl(new LoggerImplementation)
    {

    }

    Logger::~Logger()
    {

    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Functions (Factory)
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    
    void Logger::RegisterCreator(const QString& key, Creator<LogDestination>* creator)
    {
        pImpl->myFactory.RegisterCreator(key,creator);
    }

    void Logger::Create(const QString& key, const QString& groupName)
    {
        DestinationMap::iterator iterMap = pImpl->myDestinationMap.find(groupName);

        if (iterMap != pImpl->myDestinationMap.end()) // add the location to existing group
        {
            (*iterMap).second.push_back(pImpl->myFactory.Create(key));
        } else { // create new group with the desired location and add it to the map
            DestinationGroup group;
            group.push_back(pImpl->myFactory.Create(key));
            pImpl->myDestinationMap.insert(iterMap,std::pair<QString,DestinationGroup>(groupName,std::move(group)));
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Functions
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    void Logger::write(const QString& message, Level level, const QString& groupName)
    {
        DestinationMap::iterator iterMap = pImpl->myDestinationMap.find(groupName);

        if (iterMap != pImpl->myDestinationMap.end())
        {
            for (DestinationGroup::iterator iterGroup = (*iterMap).second.begin()
                ;iterGroup != (*iterMap).second.end()
                ;)
            {
                if (!(*iterGroup)) // in this case the destination was probably removed by its owner
                {
                    Q_ASSERT_X(false,"Bad LogDestination Pointer (nullptr)","void QLog::Logger::write(const QString&,Level,const QString&)");
                    iterGroup = (*iterMap).second.erase(iterGroup); // erase the bad pointer from the group
                } else {
                    (*iterGroup)->write(message,level); // pass message to current destination
                    ++iterGroup;
                }
            }
        } else {
            Q_ASSERT_X(false,"Requested DestinationGroup Not Found","void QLog::Logger::write(const QString&,Level,const QString&)");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Getters & Setters
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    
    void Logger::setLogLevel(Level level)
    {
        pImpl->myLoggerLevel = level;
    }

    Level Logger::getLogLevel()
    {
        return pImpl->myLoggerLevel;
    }

    void Logger::setDestinationGroupName(const QString& groupName)
    {
        pImpl->myLoggerDestinationGroup = groupName;
    }

    QString Logger::getDestinationGroupName()
    {
        return pImpl->myLoggerDestinationGroup;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Helper
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    Logger::Helper::~Helper()
    {
        try
        {
            write();
        }
        catch(std::exception&)
        {
            Q_ASSERT_X(false,"One Does Not Simply Throw Exceptions From A Sink...","Logger::Helper::~Helper()");
            throw;
        }
    }

    QDebug& Logger::Helper::stream()
    {
        return myDebug;
    }

    void Logger::Helper::write()
    {
        const QString level = LevelToText(myMessageLevel);
        const QString message(QString("%1 %2 %3")
            .arg(level, 6)
            .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz"))
            .arg(myStringBuffer)
            );

        Logger& logger = Logger::Instance();

        QMutexLocker lock(&logger.pImpl->myMutex); ///< &pImpl = memory location of pointee
        logger.write(message, myMessageLevel, logger.pImpl->myLoggerDestinationGroup);
    }

} // namespace QLog