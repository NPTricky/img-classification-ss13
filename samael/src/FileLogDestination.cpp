#include "stdafx.h"
#include "FileLogDestination.h"

namespace QLog
{

    FileLogDestination::FileLogDestination(const QString& filename)
        : LogDestination()
    {
        QDir directory("log");

        if (!directory.exists())
        {
            directory = QDir().current();
            directory.mkdir("log");
            directory.cd("log");
        }

        myFile.setFileName(directory.filePath(filename));
        myFile.open(QIODevice::WriteOnly | QIODevice::Text);
        myTextStream.setDevice(&myFile);
    }

    FileLogDestination::~FileLogDestination()
    {
    
    }

    void FileLogDestination::write(const QString& message, Level level)
    {
        myTextStream << message << endl;
        myTextStream.flush();
    }

} // namespace QLog
