#include "stdafx.h"
#include "SamaelMainWindow.h"
#include "SamaelApplication.h"

#include "Logger.h"
#include "TerminalWidget.h"
#include "TerminalWidgetLogDestination.h"
#include "SamaelItemModel.h"
#include "SamaelImage.h"
#include "TreeWidget.h"
#include "ToolBox.h"
#include "ComputationManagerBOW.h"
#include "ImageDatabase.h"
#include "CentralWidget.h"

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
    qRegisterMetaType<SamaelImage>("SamaelImage");

    m_TerminalWidget = terminal;

    m_imageDataBase = ImageDataBase::getInstance();

    m_computationManager = ComputationManagerBOW::getInstance(16);

    this->setMinimumSize(640, 480);

    createWidgets();   ///< instantiation of different interface elements, also known as QWidgets
    createActions();   ///< creates QActions which represent specific user commands
    createMenus();     ///< populates the MenuBar (File, Edit, Help, ...) with QActions
    createStatusBar(); ///< create and configure the QStatusBar at the bottom of the window

    QLOG_INFO() << "SamaelMainWindow - Ready!";
}

void SamaelMainWindow::createWidgets()
{
  // Central Widget
  m_CentralWidget = new CentralWidget(this);

  // Viewer Widget
  this->setCentralWidget(m_CentralWidget);

  // Terminal Widget
  m_TerminalWidget->setParent(this);
  this->addDockWidget(Qt::BottomDockWidgetArea, m_TerminalWidget);

  // Tree Widget
  m_TreeWidget = new TreeWidget(this);
  this->addDockWidget(Qt::LeftDockWidgetArea, m_TreeWidget);

  QObject::connect(m_TreeWidget, SIGNAL(saveImage(std::string, SamaelImage*)), m_imageDataBase, SLOT(addImage(std::string, SamaelImage*)));
  QObject::connect(m_TreeWidget, SIGNAL(removeImages(std::string)), m_imageDataBase, SLOT(removeImages(std::string)));

  // Toolbox Widget
  m_ToolBox = new ToolBox(this);
  this->addDockWidget(Qt::RightDockWidgetArea, m_ToolBox);

  QObject::connect(m_ToolBox, SIGNAL(getClassNames(std::vector<std::string>&)), m_imageDataBase, SLOT(getClassNames(std::vector<std::string>&)));
  QObject::connect(m_ToolBox, SIGNAL(getTrainingImages(std::map<std::string, std::vector<SamaelImage*>>&)), m_imageDataBase, SLOT(getTrainingImages(std::map<std::string, std::vector<SamaelImage*>>&)));
  QObject::connect(m_ToolBox, SIGNAL(getClassifyImages(std::map<std::string, std::vector<SamaelImage*>>&)), m_imageDataBase, SLOT(getClassifyImages(std::map<std::string, std::vector<SamaelImage*>>&)));

  #pragma WARNING(TODO: fix getFeatureDetector and related signal connections)
  //QObject::connect(m_ToolBox, SIGNAL(setFeatureDetector(int)), m_computationManager, SLOT(setFeatureDetector(int)));
  //QObject::connect(m_ToolBox, SIGNAL(getFeatureDetector(int&)), m_computationManager, SLOT(getFeatureDetector(int&)));
  QObject::connect(m_ToolBox, SIGNAL(createVocabulary(std::map<std::string, std::vector<SamaelImage*>> &)), m_computationManager, SLOT(createVocabulary(std::map<std::string, std::vector<SamaelImage*>> &)));
  QObject::connect(m_ToolBox, SIGNAL(trainClassifier(std::map<std::string, std::vector<SamaelImage*>>&)), m_computationManager, SLOT(trainClassifier(std::map<std::string, std::vector<SamaelImage*>>&)));
  QObject::connect(m_ToolBox, SIGNAL(trainSVM()), m_computationManager, SLOT(trainSVM()));
  QObject::connect(m_ToolBox, SIGNAL(classify(std::map<std::string, std::vector<SamaelImage*>>&, std::vector<std::string>&)), m_computationManager, SLOT(classify(std::map<std::string, std::vector<SamaelImage*>>&, std::vector<std::string>&)));
}

void SamaelMainWindow::createActions()
{
    // "Open" Action
    m_OpenAction = new QAction(tr("&Open"), this);
    m_OpenAction->setShortcut(Qt::CTRL + Qt::Key_O);
    m_OpenAction->setToolTip(tr("Open File(s)"));
    m_OpenAction->setStatusTip(tr("Open File(s)"));
    connect(m_OpenAction, SIGNAL(triggered()), m_TreeWidget, SLOT(openFiles()));

    // "Open Folder" Action
    m_OpenFolderAction = new QAction(tr("Open &Folder"), this);
    m_OpenFolderAction->setShortcut(Qt::CTRL + Qt::Key_F);
    m_OpenFolderAction->setToolTip(tr("Open Folder(s)"));
    m_OpenFolderAction->setStatusTip(tr("Open Folder(s)"));
    connect(m_OpenFolderAction, SIGNAL(triggered()), m_TreeWidget, SLOT(openDirectory()));

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
        m_FileMenu->addAction(m_OpenFolderAction);
        m_FileMenu->addSeparator();
        m_FileMenu->addAction(m_ExitAction);

    // "Edit" Menu
    m_EditMenu = menuBar()->addMenu(tr("&Edit"));

    // "View" Menu
    m_ViewMenu = menuBar()->addMenu(tr("&View"));
        m_ViewMenu->addAction(m_TreeWidget->toggleViewAction());
        m_ViewMenu->addAction(m_TerminalWidget->toggleViewAction());
        m_ViewMenu->addAction(m_ToolBox->toggleViewAction());
        m_ViewMenu->addSeparator();

    // "Help" Menu
    m_HelpMenu = menuBar()->addMenu(tr("&Help"));
        m_HelpMenu->addAction(m_AboutAction);
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
