#ifndef SAMAELAPPLICATION_H
#define SAMAELAPPLICATION_H

#include "SamaelMainWindow.h"
#include "Logger.h"

class SamaelApplication : public QApplication
{
    Q_OBJECT

public:
    SamaelApplication(int& argc, char** argv);
    ~SamaelApplication();

    void initialize();

private:
    std::unique_ptr<SamaelMainWindow> m_MainWindow;
    QLog::Logger& m_Logger;

    std::unique_ptr<QLog::LogDestination> m_CommandLineLogDestination;
    std::unique_ptr<QLog::LogDestination> m_MainFileLogDestination;
};

inline SamaelApplication* GetApp()
{
    return static_cast<SamaelApplication*>(qApp);
}

#endif // SAMAELAPPLICATION_H
