#include "stdafx.h"
#include "TreeWidget.h"
#include "SamaelItemModel.h"
#include "Logger.h"

TreeWidget::TreeWidget(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("TreeWidget"), QStringLiteral("Image Data Tree"))
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