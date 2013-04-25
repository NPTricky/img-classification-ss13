#include "stdafx.h"
#include "SamaelMainWindow.h"
#include "SamaelApplication.h"

#include "Logger.h"
#include "TerminalWidget.h"
#include "TerminalWidgetLogDestination.h"
#include "SamaelItemModel.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors & Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////
 
SamaelMainWindow::SamaelMainWindow(QWidget *parent)
    : QMainWindow(parent)
{

}

SamaelMainWindow::~SamaelMainWindow()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////////////////////////

void SamaelMainWindow::initialize(TerminalWidget* terminal)
{
    m_TerminalWidget = terminal;

    this->setMinimumSize(640,480);

    createActions();   ///< creates QActions which represent specific user commands
    createMenus();     ///< populates the MenuBar (File, Edit, Help, ...) with QActions
    createWidgets();   ///< instantiation of different interface elements, also known as QWidgets
    createViews();     ///< create models and their respective viewers
    createLayouts();   ///< arrange QWidgets into different layout groups to keep them in order
    createStatusBar(); ///< create and configure the QStatusBar at the bottom of the window

    QLOG_INFO() << "SamaelMainWindow - Ready!";
}

void SamaelMainWindow::createActions()
{
    // "Open" Action
    m_OpenAction = new QAction(tr("&Open"), this);
    m_OpenAction->setShortcut(Qt::CTRL + Qt::Key_O);
    m_OpenAction->setToolTip(tr("Open File(s)"));
    m_OpenAction->setStatusTip(tr("Open File(s)"));
    connect(m_OpenAction, SIGNAL(triggered()), this, SLOT(open()));

    // "Exit" Action
    m_ExitAction = new QAction(tr("&Exit"), this);
    m_ExitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    m_ExitAction->setToolTip(tr("Exit Application"));
    m_ExitAction->setStatusTip(tr("Exit Application"));
    connect(m_ExitAction, SIGNAL(triggered()), this, SLOT(exit()));

    // "About" Action
    m_AboutAction = new QAction(tr("&About"), this);
    m_AboutAction->setToolTip(tr("About Application"));
    m_AboutAction->setStatusTip(tr("About Application"));
    connect(m_AboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void SamaelMainWindow::createMenus()
{
    // "File" Menu
    m_FileMenu = menuBar()->addMenu(tr("&File"));
        m_FileMenu->addAction(m_OpenAction);
        m_FileMenu->addSeparator();
        m_FileMenu->addAction(m_ExitAction);

    // "Edit" Menu
    m_EditMenu = menuBar()->addMenu(tr("&Edit"));

    // "View" Menu
    m_ViewMenu = menuBar()->addMenu(tr("&View"));
        m_ViewMenu->addSeparator();

    // "Help" Menu
    m_HelpMenu = menuBar()->addMenu(tr("&Help"));
        m_HelpMenu->addAction(m_AboutAction);
}

void SamaelMainWindow::createWidgets()
{
    m_DockAlpha = new QDockWidget(tr("Dock Alpha"),this);
    m_DockAlpha->setObjectName(QStringLiteral("m_DockAlpha"));
    m_DockAlpha->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_DockAlphaContent = new QWidget();
    m_DockAlphaContent->setObjectName(QStringLiteral("m_DockAlphaContent"));
    m_DockAlpha->setWidget(m_DockAlphaContent);

    m_DockBeta = new QDockWidget(tr("Dock Beta"),this);
    m_DockBeta->setObjectName(QStringLiteral("m_DockBeta"));
    m_DockBeta->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_DockBetaContent = new QWidget();
    m_DockBetaContent->setObjectName(QStringLiteral("m_DockBetaContent"));
    m_DockBeta->setWidget(m_DockBetaContent);

    //m_TerminalWidget = new TerminalWidget(this);
    connect(m_TerminalWidget, SIGNAL(command(QString)), m_TerminalWidget, SLOT(result(QString)));
}

void SamaelMainWindow::createViews()
{
    m_SamaelItemModel = new SamaelItemModel(this);
    m_TreeView = new QTreeView(this);
    m_TreeView->setModel(m_SamaelItemModel);
}

void SamaelMainWindow::createLayouts()
{
    this->setCentralWidget(m_TerminalWidget);

    this->addDockWidget(Qt::LeftDockWidgetArea,m_DockAlpha);
    this->addDockWidget(Qt::RightDockWidgetArea,m_DockBeta);

    m_DockAlphaVBoxLayout = new QVBoxLayout(m_DockAlphaContent);
    m_DockAlphaVBoxLayout->setObjectName(QStringLiteral("m_DockAlphaVBoxLayout"));
    m_DockAlphaVBoxLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    m_DockAlphaVBoxLayout->addWidget(m_TreeView);
    m_DockAlphaContent->setLayout(m_DockAlphaVBoxLayout);

    m_DockBetaVBoxLayout = new QVBoxLayout(m_DockBetaContent);
    m_DockBetaVBoxLayout->setObjectName(QStringLiteral("m_DockBetaVBoxLayout"));
    m_DockBetaVBoxLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    //m_DockBetaVBoxLayout->addWidget(widgety);
    m_DockBetaContent->setLayout(m_DockBetaVBoxLayout);
}

void SamaelMainWindow::createStatusBar()
{
    m_StatusBarLabel = new QLabel(this);
    m_StatusBarLabel->setText("[QSTATUSBAR-LABEL-1]");

    this->statusBar()->addPermanentWidget(m_StatusBarLabel); // attach label to QStatusBar

    this->statusBar()->showMessage(tr("Ready"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////////////////////////

void SamaelMainWindow::open()
{
    QStringList files;

    QString	currentFileType;
    QFileInfo currentFileInfo;

    this->statusBar()->showMessage(tr("Invoked - File >> Open"));

    files = QFileDialog::getOpenFileNames(
        this,
        tr("Open File(s)"),
        QDir::currentPath(),
        tr("Image Types (*.bmp *.dib *.jpeg *.jpg *.jpe *.jp2 *.png *.pbm *.pgm *.ppm *.tiff *.tif);;" \
           "Bitmap (*.bmp *.dib);;" \
           "JPEG (*.jpeg *.jpg *.jpe *.jp2);;" \
           "Portable Network Graphics (*.png);;" \
           "Portable Image Format (*.pbm *.pgm *.ppm);;" \
           "TIFF (*.tiff *.tif);;" \
           "All Types (*.*)")
    );
 
    if (files.isEmpty()) return;

    for (QStringList::const_iterator iter = files.cbegin(); iter != files.cend(); ++iter)
    {
        currentFileInfo.setFile(*iter);

        // print some general information
        QLOG_INFO() << QString("NAME: %1 [SUFFIX: %2] - BYTES: %3")
            .arg(currentFileInfo.fileName())
            .arg(currentFileInfo.suffix())
            .arg(currentFileInfo.size())
            .toStdString().c_str();
        QLOG_INFO() << QString("PATH: %1")
            .arg(currentFileInfo.absolutePath())
            .toStdString().c_str();
        QLOG_INFO() << QString("READ: %1 - WRITE: %2\n")
            .arg(currentFileInfo.isReadable())
            .arg(currentFileInfo.isWritable())
            .toStdString().c_str();

        // DO STUFF
    }
}

void SamaelMainWindow::exit()
{
    auto input = QMessageBox::Yes; ///< shortcut until more security is required
    //auto input = QMessageBox::question(
    //    this,
    //    tr("Samael Image Classification"),
    //    tr("Do you really want to quit "Samael"?\nAll unsaved process will be lost."),
    //    QMessageBox::Yes | QMessageBox::No,
    //    QMessageBox::No
    //    );
    if (input == QMessageBox::Yes)
    {
        GetApp()->quit();
    }
}

void SamaelMainWindow::about()
{

}
