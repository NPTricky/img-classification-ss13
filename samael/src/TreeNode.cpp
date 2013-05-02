#include "stdafx.h"
#include "TreeNode.h"

TreeNode::TreeNode(const QVector<QVariant>& data, TreeNode* parent /*= nullptr*/)
    : m_Parent(parent)
    , m_Data(data)
{

}

TreeNode::~TreeNode()
{
    qDeleteAll(m_Children);
}

void TreeNode::appendChild(TreeNode* child)
{
    m_Children.append(child);
}

TreeNode* TreeNode::getChild(int row)
{
    return m_Children.value(row);
}

bool TreeNode::hasChildren(const QModelIndex &parent /*= QModelIndex( ) */) const
{
    return parent.isValid() ? false : (childCount() > 0);
}

QVector<TreeNode*> TreeNode::getChildren()
{
    return m_Children;
}

int TreeNode::childCount() const
{
    return m_Children.size();
}

int TreeNode::columnCount() const
{
    return m_Data.size();
}

QVariant TreeNode::data(int column) const
{
    return m_Data.value(column);
}

int TreeNode::row() const
{
    if (m_Parent)
        return m_Parent->getChildren().indexOf(const_cast<TreeNode*>(this));

    return 0;
}

TreeNode* TreeNode::parent()
{
    return m_Parent;
}
