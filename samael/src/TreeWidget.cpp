#include "stdafx.h"
#include "TreeWidget.h"
#include "SamaelItemModel.h"
#include "Logger.h"

TreeWidget::TreeWidget(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("TreeWidget"), QStringLiteral("Data"))
{
    // create the data model
    m_SamaelItemModel = new SamaelItemModel(m_ContentWidget);

    // configure the tree view
    m_TreeView = new QTreeView(m_ContentWidget);
    m_TreeView->header()->setSectionsClickable(false);
    m_TreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_TreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_TreeView->setModel(m_SamaelItemModel);

    // configure the layout of this widget
    m_Layout = new QVBoxLayout(m_ContentWidget);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_TreeView);
    finalise(m_Layout);

    QLOG_INFO() << "TreeWidget - Ready!";
}

TreeWidget::~TreeWidget()
{

}

void TreeWidget::open()
{
    QStringList files;

    QFileInfo info;

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
        info.setFile(*iter);

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

        // DO STUFF
    }
}