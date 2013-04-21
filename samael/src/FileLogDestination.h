#ifndef FILELOGDESTINATION_H
#define FILELOGDESTINATION_H

#include "LogDestination.h"

namespace QLog
{

    class FileLogDestination : public LogDestination
    {
    public:
        FileLogDestination(const QString& filename = "log.txt");
        ~FileLogDestination();

        virtual void write(const QString& message, Level level);

    private:
        QFile myFile;
        QTextStream myTextStream;
    };

} // namespace QLog

#endif // FILELOGDESTINATION_H
