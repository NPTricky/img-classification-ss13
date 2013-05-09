#include "stdafx.h"
#include "TreeWidget.h"
#include "SamaelItemModel.h"
#include "Logger.h"
#include "SamaelImage.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors & Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////

TreeWidget::TreeWidget(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("TreeWidget"), QStringLiteral("Data"))
{
    // create the data model
    m_SamaelItemModel = new SamaelItemModel(m_ContentWidget);

    // create file type filter
    m_Filters << "*.bmp" << "*.dib" << "*.jpeg" << "*.jpg"<< "*.jpe"<< "*.jp2" << "*.png" << "*.pbm" << "*.pgm" << "*.ppm" << "*.tiff" << "*.tif";

    // configure the tree view
    m_TreeView = new QTreeView(m_ContentWidget);
    m_TreeView->header()->setSectionsClickable(false);
    m_TreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_TreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_TreeView->setModel(m_SamaelItemModel);

    // create actions
    createActions();

    // configure the toolbar
    m_ToolBar = new QToolBar(m_ContentWidget);
    m_ToolBar->addAction(m_ExpandAction);
    m_ToolBar->addAction(m_CollapseAction);

    // configure the layout of this widget
    m_Layout = new QVBoxLayout(m_ContentWidget);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_ToolBar);
    m_Layout->addWidget(m_TreeView);
    finalise(m_Layout);

    QLOG_INFO() << "TreeWidget - Ready!";
}

TreeWidget::~TreeWidget()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////////////////////////

QModelIndex TreeWidget::insertDefault(QString path, const QModelIndex &parent /*= QModelIndex( ) */)
{
    return QModelIndex();
}

void TreeWidget::load(QDir directory)
{
    if (!directory.exists())
        return;

    QModelIndex root = m_SamaelItemModel->index(0,0);

    QModelIndexList matches = m_SamaelItemModel->match(root, Qt::DisplayRole, directory.dirName(), 1, Qt::MatchRecursive);
    
    QModelIndex parent = matches.isEmpty() ? /*create dir node*/ : matches[0];

    // load all files within the directory
    directory.setNameFilters(m_Filters);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList files = directory.entryList();

    for (int i = 0; i < files.count(); i++)
    {
        //fileList[i]
    }

    // recurse subdirectories
    directory.setNameFilters(QStringList());
    directory.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QFileInfo info;
    QStringList directories = directory.entryList();

    for (int i = 0; i < directories.size(); ++i)
    {
        QString newPath = QString("%1/%2").arg(directory.absolutePath()).arg(directories.at(i));
        load(QDir(newPath));
    }
}

void TreeWidget::load(QString file, const QModelIndex &parent)
{
    if (file.isEmpty() || !parent.isValid())
        return;

    QFileInfo info(file);

    // print some general information
    QLOG_INFO() << QString("NAME: %1 [SUFFIX: %2] - BYTES: %3")
        .arg(info.fileName())
        .arg(info.suffix())
        .arg(info.size())
        .toStdString().c_str();
    QLOG_INFO() << QString("PATH: %1")
        .arg(info.absolutePath())
        .toStdString().c_str();
    QLOG_INFO() << QString("READ: %1 - WRITE: %2\n")
        .arg(info.isReadable())
        .arg(info.isWritable())
        .toStdString().c_str();

    // do the loading
    QVector<QVariant> data;
    //data.append(QVariant::fromValue(SamaelNodeMetadata()));
    data.append(QVariant::fromValue(SamaelImage(file)));
    //m_SamaelItemModel->insertColumns(0,data,parent);
}

void TreeWidget::createActions()
{
    // "Expand" Action
    m_ExpandAction = new QAction(tr("&Expand Tree"), this);
    m_ExpandAction->setObjectName(QStringLiteral("m_ExpandAction"));
    m_ExpandAction->setShortcut(Qt::CTRL + Qt::Key_O);
    m_ExpandAction->setText(tr("Expand Tree"));
    m_ExpandAction->setToolTip(tr("Expand Tree"));
    m_ExpandAction->setStatusTip(tr("Expand Tree"));
    connect(m_ExpandAction, SIGNAL(triggered()), m_TreeView, SLOT(expandAll()));

    QIcon iconExpand;
    iconExpand.addFile(":/content/icons/treewidget_expand.svg", QSize(), QIcon::Normal, QIcon::Off);
    m_ExpandAction->setIcon(iconExpand);

    // "Collapse" Action
    m_CollapseAction = new QAction(tr("&Collapse Tree"), this);
    m_CollapseAction->setObjectName(QStringLiteral("m_CollapseAction"));
    m_CollapseAction->setShortcut(Qt::CTRL + Qt::Key_O);
    m_CollapseAction->setText(tr("Collapse Tree"));
    m_CollapseAction->setToolTip(tr("Collapse Tree"));
    m_CollapseAction->setStatusTip(tr("Collapse Tree"));
    connect(m_CollapseAction, SIGNAL(triggered()), m_TreeView, SLOT(collapseAll()));

    QIcon iconCollapse;
    iconCollapse.addFile(":/content/icons/treewidget_collapse.svg", QSize(), QIcon::Normal, QIcon::Off);
    m_CollapseAction->setIcon(iconCollapse);
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

    QString dir = QFileInfo(files[0]).dir().dirName();

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

    if (directory.isEmpty())
        return;

    load(QDir(directory));
}
