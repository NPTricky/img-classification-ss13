#include "stdafx.h"
#include "SamaelMainWindow.h"
#include "SamaelApplication.h"

#include "Logger.h"
#include "TerminalWidget.h"
#include "TerminalWidgetLogDestination.h"
#include "SamaelItemModel.h"
#include "SamaelImage.h"
#include "TreeWidget.h"

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

    this->setMinimumSize(640,480);

    createWidgets();   ///< instantiation of different interface elements, also known as QWidgets
    createActions();   ///< creates QActions which represent specific user commands
    createMenus();     ///< populates the MenuBar (File, Edit, Help, ...) with QActions
    createStatusBar(); ///< create and configure the QStatusBar at the bottom of the window

    QLOG_INFO() << "SamaelMainWindow - Ready!";
}

void SamaelMainWindow::createWidgets()
{
    // Viewer Widget
    this->setCentralWidget(m_TerminalWidget); ///< viewer plz

    // Terminal Widget
    this->addDockWidget(Qt::RightDockWidgetArea,m_TerminalWidget);

    // Tree Widget
    m_TreeWidget = new TreeWidget(this);
    this->addDockWidget(Qt::LeftDockWidgetArea,m_TreeWidget);
}

void SamaelMainWindow::createActions()
{
    // "Open" Action
    m_OpenAction = new QAction(tr("&Open"), this);
    m_OpenAction->setShortcut(Qt::CTRL + Qt::Key_O);
    m_OpenAction->setToolTip(tr("Open File(s)"));
    m_OpenAction->setStatusTip(tr("Open File(s)"));
    connect(m_OpenAction, SIGNAL(triggered()), m_TreeWidget, SLOT(open()));

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
