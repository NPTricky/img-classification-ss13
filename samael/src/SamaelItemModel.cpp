#include "stdafx.h"
#include "SamaelItemModel.h"
#include "TreeNode.h"

SamaelItemModel::SamaelItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    #pragma WARNING("das wird später noch besser gehen als so..")
    QVector<QVariant> vector;
    vector << "Hello";
    m_RootNode = new TreeNode(vector);
}

SamaelItemModel::~SamaelItemModel()
{
    delete m_RootNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Basics
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

bool SamaelItemModel::hasChildren( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    return parent.isValid() ? false : (rowCount() > 0);
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
    //if (!index.isValid())
    //    return 0;

    //return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

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

bool SamaelItemModel::setData( const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole */ )
{
    throw std::exception("The method or operation is not implemented.");
}

bool SamaelItemModel::insertRows( int row, int count, const QModelIndex &parent /*= QModelIndex( ) */ )
{
    throw std::exception("The method or operation is not implemented.");
}

bool SamaelItemModel::removeRows( int row, int count, const QModelIndex &parent /*= QModelIndex( ) */ )
{
    throw std::exception("The method or operation is not implemented.");
}
