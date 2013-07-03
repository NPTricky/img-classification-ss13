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

    m_computationManager = ComputationManagerBOW::getInstance(1000);

    this->setMinimumSize(640, 480);

    createWidgets();   ///< instantiation of different interface elements, also known as QWidgets
    createActions();   ///< creates QActions which represent specific user commands
    createMenus();     ///< populates the MenuBar (File, Edit, Help, ...) with QActions
    createStatusBar(); ///< create and configure the QStatusBar at the bottom of the window

    QLOG_INFO() << "SamaelMainWindow - Ready!";
}

void SamaelMainWindow::createWidgets()
{
  QObject::connect(m_computationManager, SIGNAL(getClassNames(std::vector<std::string>&)), m_imageDataBase, SLOT(getClassNames(std::vector<std::string>&)));
  QObject::connect(m_computationManager, SIGNAL(getTrainingImages(std::map<std::string, std::vector<SamaelImage*>>&)), m_imageDataBase, SLOT(getTrainingImages(std::map<std::string, std::vector<SamaelImage*>>&)));
  QObject::connect(m_computationManager, SIGNAL(getClassifyImages(std::map<std::string, std::vector<SamaelImage*>>&)), m_imageDataBase, SLOT(getClassifyImages(std::map<std::string, std::vector<SamaelImage*>>&)));

  // Central Widget
  m_CentralWidget = new CentralWidget(this);

  // Viewer Widget
  this->setCentralWidget(m_CentralWidget);
  QObject::connect(m_computationManager, SIGNAL(displayMatrix(cv::Mat&)), m_CentralWidget, SLOT(displayMatrix(cv::Mat&)));

  // Terminal Widget
  m_TerminalWidget->setParent(this);
  this->addDockWidget(Qt::BottomDockWidgetArea, m_TerminalWidget);

  // Tree Widget
  m_TreeWidget = new TreeWidget(this);
  this->addDockWidget(Qt::LeftDockWidgetArea, m_TreeWidget);
  QObject::connect(m_TreeWidget, SIGNAL(addImageToDatabase(std::string, SamaelImage*)), m_imageDataBase, SLOT(addImage(std::string, SamaelImage*)));
  QObject::connect(m_TreeWidget, SIGNAL(removeClassFromDatabase(std::string)), m_imageDataBase, SLOT(removeImages(std::string)));

  // Toolbox Widget
  m_ToolBox = new ToolBox(this);
  this->addDockWidget(Qt::RightDockWidgetArea, m_ToolBox);
  QObject::connect(m_ToolBox, SIGNAL(doClassification()), m_computationManager, SLOT(doClassification()));
}

void SamaelMainWindow::createActions()
{
    // "Open" Action
    m_OpenAction = new QAction(tr("&Open File(s)"), this);
    m_OpenAction->setShortcut(Qt::CTRL + Qt::Key_O);
    m_OpenAction->setToolTip(tr("Open File(s)"));
    m_OpenAction->setStatusTip(tr("Open File(s)"));
    connect(m_OpenAction, SIGNAL(triggered()), m_TreeWidget, SLOT(openFiles()));

    // "Open Folder" Action
    m_OpenFolderAction = new QAction(tr("Open &Folder (Recursive)"), this);
    m_OpenFolderAction->setShortcut(Qt::CTRL + Qt::Key_F);
    m_OpenFolderAction->setToolTip(tr("Open Folder (Recursive)"));
    m_OpenFolderAction->setStatusTip(tr("Open Folder (Recursive)"));
    connect(m_OpenFolderAction, SIGNAL(triggered()), m_TreeWidget, SLOT(openDirectory()));

    // "Open Confusion Matrix" Action
    m_OpenConfusionMatrixAction = new QAction(tr("Open Confusion &Matrix"), this);
    m_OpenConfusionMatrixAction->setShortcut(Qt::CTRL + Qt::Key_M);
    m_OpenConfusionMatrixAction->setToolTip(tr("Open Confusion Matrix"));
    m_OpenConfusionMatrixAction->setStatusTip(tr("Open Confusion Matrix"));
    connect(m_OpenConfusionMatrixAction, SIGNAL(triggered()), m_computationManager, SLOT(loadConfusionMatrix()));

    // "Save Confusion Matrix" Action
    m_SaveConfusionMatrixAction = new QAction(tr("Save Confusion &Matrix"), this);
    m_SaveConfusionMatrixAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_M);
    m_SaveConfusionMatrixAction->setToolTip(tr("Save Confusion Matrix"));
    m_SaveConfusionMatrixAction->setStatusTip(tr("Save Confusion Matrix"));
    connect(m_SaveConfusionMatrixAction, SIGNAL(triggered()), m_computationManager, SLOT(saveConfusionMatrix()));

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

    // "Toggle Main View" Action
    m_ToggleMainViewAction = new QAction(tr("Toggle Main View"), this);
    m_ToggleMainViewAction->setShortcut(Qt::CTRL + Qt::Key_Space);
    m_ToggleMainViewAction->setToolTip(tr("Toggle Main View"));
    m_ToggleMainViewAction->setStatusTip(tr("Toggle Main View"));
    connect(m_ToggleMainViewAction, SIGNAL(triggered()), m_CentralWidget, SLOT(toggleWidget()));
}

void SamaelMainWindow::createMenus()
{
    // "File" Menu
    m_FileMenu = menuBar()->addMenu(tr("&File"));
        m_FileMenu->addAction(m_OpenAction);
        m_FileMenu->addAction(m_OpenFolderAction);
        m_FileMenu->addSeparator();
        m_FileMenu->addAction(m_OpenConfusionMatrixAction);
        m_FileMenu->addAction(m_SaveConfusionMatrixAction);
        m_FileMenu->addSeparator();
        m_FileMenu->addAction(m_ExitAction);

    // "Edit" Menu
    m_EditMenu = menuBar()->addMenu(tr("&Edit"));

    // "View" Menu
    m_ViewMenu = menuBar()->addMenu(tr("&View"));
        m_ViewMenu->addAction(m_ToggleMainViewAction);
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
