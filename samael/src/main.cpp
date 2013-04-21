#include "stdafx.h"
#include "SamaelApplication.h"

//#if !defined(NDEBUG)
//#define _CRTDBG_MAP_ALLOC // definition for c++ native visual studio memory leak detection
//#include <stdlib.h>
//#include <crtdbg.h>
//#endif

int runApplication(int argc, char * argv[])
{
    setlocale(LC_ALL, NULL); // set application locale to system defaults

    SamaelApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); // set translation file (*.ts) character encoding

    // meta information for the executable
    a.setApplicationName("Samael");
    a.setOrganizationName("AG Bildverarbeitung");
    a.setOrganizationDomain("http://wwwisg.cs.uni-magdeburg.de/bv/index.php?article_id=14");

    // additional library directories for the executable
    QString pathProject(QDir::currentPath());
    QString pathBin(pathProject.append("/bin/"));
    a.addLibraryPath(pathBin);

    // deferred initialization of application
    a.initialize();

    return a.exec();
}

int main(int argc, char *argv[])
{
#if defined(_CRTDBG_MAP_ALLOC)
    //_CrtSetBreakAlloc(42); // insert CRT leak number from within curled brackets (e.g. {42})
#endif

    Q_INIT_RESOURCE(samael); // initialization of ressource file (*.qrc)

    int result = runApplication(argc, argv);

    Q_CLEANUP_RESOURCE(samael);

    // last (!) function call
#if defined(_CRTDBG_MAP_ALLOC)
    _CrtDumpMemoryLeaks(); // dump memory leaks into Visual Studio output 
#endif

    return result; // I'M OUT!
}
