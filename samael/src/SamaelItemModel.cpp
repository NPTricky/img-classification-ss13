#include "stdafx.h"
#include "SamaelItemModel.h"

SamaelItemModel::SamaelItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    #pragma WARNING("das wird später noch besser gehen als so..")
    QVector<QVariant> vector;
    vector << "Hallo" << "Tim";
    m_RootNode = new TreeNode(vector);
}

SamaelItemModel::~SamaelItemModel()
{
    delete m_RootNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
//bool QAbstractItemModel::hasIndex(int row, int column, const QModelIndex &parent) const
//{
//    if (row < 0 || column < 0)
//        return false;
//    return row < rowCount(parent) && column < columnCount(parent);
//}

QModelIndex SamaelItemModel::index( int row, int column, const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    throw std::exception("The method or operation is not implemented.");

    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex SamaelItemModel::parent( const QModelIndex &child ) const
{
    throw std::exception("The method or operation is not implemented.");

    //return QModelIndex();
}

QModelIndex SamaelItemModel::sibling( int row, int column, const QModelIndex &idx ) const
{
    throw std::exception("The method or operation is not implemented.");

    //if (!idx.isValid() || column != 0 || row >= m_Data->size())
    //    return QModelIndex();

    //return createIndex(row, 0);

    /////  or (???) fo realz? 

    //return hasIndex(row, column, idx) ? createIndex(row, column) : QModelIndex();
}

bool SamaelItemModel::hasChildren( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    throw std::exception("The method or operation is not implemented.");

    //return parent.isValid() ? false : (rowCount() > 0);
}

int SamaelItemModel::rowCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    throw std::exception("The method or operation is not implemented.");

    //return parent.isValid() ? 0 : m_Data->size();
}

int SamaelItemModel::columnCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    throw std::exception("The method or operation is not implemented.");

    //return parent.isValid() ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////

Qt::ItemFlags SamaelItemModel::flags( const QModelIndex &index ) const
{
    throw std::exception("The method or operation is not implemented.");
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SamaelItemModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ ) const
{
    throw std::exception("The method or operation is not implemented.");

    /// see Qt::ItemDataRole for different roles
    switch (role)
    {
    case Qt::DisplayRole:
        break;
    case Qt::ToolTipRole:
        break;
    case Qt::AccessibleTextRole:
        break;
    case Qt::AccessibleDescriptionRole:
        break;
    default:
        break;
    }

    return QVariant();
}

QVariant SamaelItemModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */ ) const
{
    throw std::exception("The method or operation is not implemented.");

    /// see Qt::ItemDataRole for different roles
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch( section )
        {
        case 0:
            return QString( "Element" );
        case 1:
            return QString( "Annotation" );
        default:
            break;
        }
    }

    return QVariant();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
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
