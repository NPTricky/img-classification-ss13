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
    SamaelMainWindow* m_MainWindow;
    QLog::Logger& m_Logger;
};

inline SamaelApplication* GetApp()
{
    return static_cast<SamaelApplication*>(qApp);
}

#endif // SAMAELAPPLICATION_H
