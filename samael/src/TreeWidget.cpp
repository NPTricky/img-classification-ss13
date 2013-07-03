#include "stdafx.h"
#include "TreeWidget.h"
#include "Logger.h"
#include "SamaelImage.h"
#include "FileExplorerListView.h"
#include "FileExplorerTreeProxyModel.h"
#include "FileExplorerListProxyModel.h"
#include "SamaelApplication.h"
#include "ThumbnailIconProvider.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors & Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////

TreeWidget::TreeWidget(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("TreeWidget"), QStringLiteral("File Explorer"))
{
    this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_FiltersRegExp = QRegExp("(bmp|dib|jpeg|jpg|jpe|jp2|png|pbm|pgm|ppm|tiff|tif)$", Qt::CaseInsensitive);
    m_FiltersByName = QStringList() \
        << "*.bmp" << "*.dib" << "*.jpeg" \
        << "*.jpg" << "*.jpe" << "*.jp2" \
        << "*.png" << "*.pbm" << "*.pgm" \
        << "*.ppm" << "*.tiff" << "*.tif";

    // create the data model
    m_FileSystemModel = new QFileSystemModel(m_ContentWidget);
    m_FileSystemModel->setRootPath("");

    // create the icon provider
    m_IconProvider = new ThumbnailIconProvider(m_FiltersRegExp);
    m_FileSystemModel->setIconProvider(m_IconProvider);

    // create the respective proxy models
    m_TreeProxyModel = new FileExplorerTreeProxyModel(m_ContentWidget);
    m_TreeProxyModel->setSourceModel(m_FileSystemModel);
    m_ListProxyModel = new FileExplorerListProxyModel(m_ContentWidget);
    m_ListProxyModel->setSourceModel(m_FileSystemModel);

    // configure the tree view
    m_TreeView = new QTreeView(m_ContentWidget);
    m_TreeView->setAlternatingRowColors(true);
    m_TreeView->setModel(m_TreeProxyModel);

    // configure the list view
    m_ListView = new FileExplorerListView(m_ContentWidget);
    m_ListView->setModel(m_ListProxyModel);

    // create actions
    createActions();

    // configure the toolbar
    m_ToolBar = new QToolBar(m_ContentWidget);
    m_ToolBar->addAction(m_CollapseAction);
    m_ToolBar->addAction(m_ScrollToProjectAction);
    m_ToolBar->addAction(m_ScrollToSelectionAction);

    // configure the layout of this widget
    m_Layout = new QVBoxLayout(m_ContentWidget);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_ToolBar);
    m_Layout->addWidget(m_TreeView);
    m_Layout->addWidget(m_ListView);
    finalise(m_Layout);

    //connect(m_TreeView, SIGNAL(clicked(QModelIndex)), m_ListView, SLOT(setRootIndexProxy(QModelIndex)));
    connect(m_TreeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), m_ListView, SLOT(onCurrentChanged(QModelIndex,QModelIndex)));

    QLOG_INFO() << "TreeWidget - Ready!";
}

TreeWidget::~TreeWidget()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////////////////////////

void TreeWidget::load(QDir directory)
{
    if (!directory.exists())
        return;

    // load all files within the directory
    directory.setNameFilters(m_FiltersByName);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList files = directory.entryList();

    for (int i = 0; i < files.count(); i++)
    {
        load(directory.absoluteFilePath(files[i]));
    }

    QLOG_INFO_NOCONTEXT() << "load(QDir) loaded" << files.size() << "files from" << directory.dirName();

    // recurse subdirectories
    directory.setNameFilters(QStringList());
    directory.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList directories = directory.entryList();
    for (int i = 0; i < directories.size(); i++)
    {
        QLOG_INFO_NOCONTEXT() << "load(QDir) is recursing into the subdirectory" << directories[i] << "from" << directory.dirName();
        QString path = directory.absolutePath().append("/" + directories[i]);
        load(QDir(path));
    }
}

void TreeWidget::load(QString path)
{
    QFileInfo file(path);

    if (!file.exists())
        return;

    // check for supported file format by opencv/imread
    if (!m_FiltersRegExp.exactMatch(file.suffix()))
        return;

    // print some general information
    //QLOG_INFO() << QString("NAME: %1 [SUFFIX: %2] - BYTES: %3")
    //    .arg(info.fileName())
    //    .arg(info.suffix())
    //    .arg(info.size())
    //    .toStdString().c_str();
    //QLOG_INFO() << QString("PATH: %1")
    //    .arg(info.absolutePath())
    //    .toStdString().c_str();
    //QLOG_INFO() << QString("READ: %1 - WRITE: %2\n")
    //    .arg(info.isReadable())
    //    .arg(info.isWritable())
    //    .toStdString().c_str();

    SamaelImage *image = new SamaelImage(path);

    std::string className = file.dir().dirName().toStdString();

    emit addImageToDatabase(className, image);
}

void TreeWidget::createActions()
{
    // "Collapse" Action
    m_CollapseAction = new QAction(tr("Collapse &Tree"), this);
    m_CollapseAction->setObjectName(QStringLiteral("m_CollapseAction"));
    //m_CollapseAction->setShortcut(Qt::CTRL + Qt::Key_T);
    m_CollapseAction->setText(tr("Collapse Tree"));
    m_CollapseAction->setToolTip(tr("Collapse Tree"));
    m_CollapseAction->setStatusTip(tr("Collapse Tree"));
    connect(m_CollapseAction, SIGNAL(triggered()), m_TreeView, SLOT(collapseAll()));

    QIcon iconCollapse;
    iconCollapse.addFile(":/content/icons/treewidget_collapse.svg", QSize(), QIcon::Normal, QIcon::Off);
    m_CollapseAction->setIcon(iconCollapse);

    // "Scroll To Selection" Action
    m_ScrollToSelectionAction = new QAction(tr("&Focus On Selection"),this);
    m_ScrollToSelectionAction->setObjectName(QStringLiteral("m_ScrollToSelectionAction"));
    //m_ScrollToSelectionAction->setShortcut(Qt::CTRL + Qt::Key_S);
    m_ScrollToSelectionAction->setText(tr("Focus On Selection"));
    m_ScrollToSelectionAction->setToolTip(tr("Focus On Selection"));
    m_ScrollToSelectionAction->setStatusTip(tr("Focus On Selection"));
    connect(m_ScrollToSelectionAction, SIGNAL(triggered()), this, SLOT(scrollToSelection()));

    QIcon iconScrollToSelection;
    iconScrollToSelection.addFile(":/content/icons/treewidget_scrolltoselection.svg", QSize(), QIcon::Normal, QIcon::Off);
    m_ScrollToSelectionAction->setIcon(iconScrollToSelection);

    // "Scroll To Project Directory" Action
    m_ScrollToProjectAction = new QAction(tr("&Scroll To Project Directory"),this);
    m_ScrollToProjectAction->setObjectName(QStringLiteral("m_ScrollToProjectAction"));
    //m_ScrollToProjectAction->setShortcut(Qt::CTRL + Qt::Key_P);
    m_ScrollToProjectAction->setText(tr("Scroll To Project Directory"));
    m_ScrollToProjectAction->setToolTip(tr("Scroll To Project Directory"));
    m_ScrollToProjectAction->setStatusTip(tr("Scroll To Project Directory"));
    connect(m_ScrollToProjectAction, SIGNAL(triggered()), this, SLOT(scrollToProject()));

    QIcon iconScrollToProject;
    iconScrollToProject.addFile(":/content/icons/treewidget_scrolltoproject.svg", QSize(), QIcon::Normal, QIcon::Off);
    m_ScrollToProjectAction->setIcon(iconScrollToProject);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Slots
///////////////////////////////////////////////////////////////////////////////////////////////////

void TreeWidget::openFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(
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

    if (files.isEmpty())
        return;

    for (QStringList::const_iterator iter = files.cbegin(); iter != files.cend(); ++iter)
    {
        load(*iter);
    }
}

void TreeWidget::openDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(
        this,
        tr("Open Folder(s)"),
        QDir::currentPath()
        );

    load(QDir(directory));
}

void TreeWidget::scrollToProject()
{
    QModelIndex target = m_TreeProxyModel->mapFromSource(m_FileSystemModel->index(GetApp()->applicationDirPath()));
    m_TreeView->setCurrentIndex(target);
    scrollToSelection();
}

void TreeWidget::scrollToSelection()
{
    m_TreeView->scrollTo(m_TreeView->currentIndex(),QAbstractItemView::PositionAtTop);
}
