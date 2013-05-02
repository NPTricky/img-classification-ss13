#include "stdafx.h"
#include "SamaelItemModel.h"
#include "TreeNode.h"

SamaelItemModel::SamaelItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    // initialize a root node
    m_RootNode = new TreeNode(QString("TREE ROOT"));

    QVector<QVariant> vector1;
    vector1 << "CHILD1_OF_ROOT";
    m_RootNode->appendChild(new TreeNode(vector1,m_RootNode));

    QVector<QVariant> vector2;
    vector2 << "CHILD2_OF_ROOT";
    auto tmp = new TreeNode(vector2,m_RootNode);    
    m_RootNode->appendChild(tmp);

    QVector<QVariant> vector3;
    vector3 << "TEST";
    tmp->appendChild(new TreeNode(vector3,tmp));
}

SamaelItemModel::~SamaelItemModel()
{
    // item model will recursively delete the full tree
    delete m_RootNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Item Model Basics
///////////////////////////////////////////////////////////////////////////////////////////////////

QModelIndex SamaelItemModel::index( int row, int column, const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    if (!hasIndex(row,column,parent))
        return QModelIndex();

    TreeNode* tmpParent;
    tmpParent = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : m_RootNode;

    TreeNode* tmpChild = tmpParent->getChild(row);
    return tmpChild ? createIndex(row, column, tmpChild) : QModelIndex();
}

QModelIndex SamaelItemModel::parent( const QModelIndex &child ) const
{
    if (!child.isValid())
        return QModelIndex();

    TreeNode* tmpChild = static_cast<TreeNode*>(child.internalPointer());
    TreeNode* tmpParent = tmpChild->parent();

    if (tmpParent == m_RootNode)
        return QModelIndex();

    return createIndex(tmpParent->row(), 0, tmpParent);
}

int SamaelItemModel::rowCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    if (parent.column() > 0)
        return 0;

    TreeNode* tmpParent;
    tmpParent = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : m_RootNode;

    return tmpParent->childCount();
}

int SamaelItemModel::columnCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    return parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer())->columnCount() : m_RootNode->columnCount();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Metadata & Data
///////////////////////////////////////////////////////////////////////////////////////////////////

Qt::ItemFlags SamaelItemModel::flags( const QModelIndex &index ) const
{
    return QAbstractItemModel::flags(index);
}

QVariant SamaelItemModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ ) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    //switch (role)
    //{
    //case Qt::DisplayRole:
    //    break;
    //case Qt::ToolTipRole:
    //    break;
    //case Qt::AccessibleTextRole:
    //    break;
    //case Qt::AccessibleDescriptionRole:
    //    break;
    //default:
    //    break;
    //}
    
    return static_cast<TreeNode*>(index.internalPointer())->data(index.column());
}

QVariant SamaelItemModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */ ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_RootNode->data(section);

    return QVariant();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Editability
///////////////////////////////////////////////////////////////////////////////////////////////////

bool SamaelItemModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole */)
{
    throw std::exception("The method or operation is not implemented.");
}

bool SamaelItemModel::insertRows(int row, int count, const QModelIndex &parent /*= QModelIndex( ) */)
{
    if (!parent.isValid())
        return false;

    TreeNode* node = static_cast<TreeNode*>(parent.internalPointer());

    beginInsertRows(parent, row, row + count - 1);
    
    for (int i = row; i < (row + count - 1); i++)
        node->insertChild(i, new TreeNode(QVector<QVariant>(), node));

    endInsertRows();

    return true;
}

bool SamaelItemModel::insertRows(int row, const QVector<QVariant>& data, const QModelIndex &parent /*= QModelIndex()*/)
{
    if (data.isEmpty() || !parent.isValid())
        return false;

    int count = data.size();
    TreeNode* node = static_cast<TreeNode*>(parent.internalPointer());
    QVector<QVariant>::const_iterator iter = data.cbegin();
    
    beginInsertRows(parent, row, row + count - 1);

    for (int i = row; i < (row + count - 1); i++)
    {
        node->insertChild(i, new TreeNode((*iter), node));
        ++iter;
    }

    endInsertRows();

    return true;
}

bool SamaelItemModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex( ) */)
{
    if (!parent.isValid())
        return false;

    TreeNode* node = static_cast<TreeNode*>(parent.internalPointer());

    beginRemoveRows(parent, row, row + count - 1);

    for (int i = row; i < (row + count - 1); i++)
        node->removeChild(i);

    endRemoveRows();

    return true;
}
