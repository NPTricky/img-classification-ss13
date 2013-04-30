#include "stdafx.h"
#include "TreeWidget.h"
#include "SamaelItemModel.h"

TreeWidget::TreeWidget(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("Tree Widget Name"), QStringLiteral("Tree Widget Title"))
    , m_SamaelItemModel(new SamaelItemModel(this))
    , m_TreeView(new QTreeView(this))
{
    m_TreeView->setModel(m_SamaelItemModel);
    m_TreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

TreeWidget::~TreeWidget()
{

}