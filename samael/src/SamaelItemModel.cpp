#include "stdafx.h"
#include "SamaelItemModel.h"
#include "TreeNode.h"
#include "SamaelImage.h"

SamaelItemModel::SamaelItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    // initialize a root node
    m_RootNode = new TreeNode("TREE ROOT");
#pragma WARNING(DELETE: Test Code!)
    auto tmp1 = new TreeNode("CHILD1_OF_ROOT",m_RootNode);
    m_RootNode->appendChild(tmp1);
    tmp1->appendChild(new TreeNode("TESTA",tmp1));
    tmp1->appendChild(new TreeNode("TESTB",tmp1));

    auto tmp2 = new TreeNode("CHILD2_OF_ROOT",m_RootNode);    
    m_RootNode->appendChild(tmp2);
    tmp2->appendChild(new TreeNode("TEST1",tmp2));
    tmp2->appendChild(new TreeNode("TEST2",tmp2));
    tmp2->appendChild(new TreeNode("TEST3",tmp2));

    auto tmp3 = new TreeNode("TEST4",tmp2);    
    tmp2->appendChild(tmp3);
    tmp3->appendChild(new TreeNode("TESTX",tmp3));
    tmp3->appendChild(new TreeNode("TESTY",tmp3));
    tmp3->appendChild(new TreeNode("TESTZ",tmp3));
}

SamaelItemModel::~SamaelItemModel()
{
    // item model will recursively delete the full tree
    delete m_RootNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Item Model Basics
///////////////////////////////////////////////////////////////////////////////////////////////////

// Root Item (Empty)
// |
// |--- Adam (row = 0) 
// |   |
// |   |--- ChildA (row = 0)
// |   |    | 
// |   |    |--- ChildX (row = 0)
// |   |
// |   |--- ChildB (row = 1)
// |   |
// |   |--- ChildC (row = 2)
// |
// |--- Eva (row = 1)
// .
// .
// 
// Row Count = Children - Number of Child Nodes of a Node
// Column Count = Data - Number of QVariant within the QVector<QVariant> of a Node

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

    return tmpParent->rowCount();
}

int SamaelItemModel::columnCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    return parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer())->columnCount() : m_RootNode->columnCount();
}

bool SamaelItemModel::hasChildren( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    return (rowCount(parent) > 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Metadata & Data
///////////////////////////////////////////////////////////////////////////////////////////////////

Qt::ItemFlags SamaelItemModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return 0;

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    SamaelImage image = qvariant_cast<SamaelImage>(node->data(index.column()));
#pragma WARNING(TODO: Aw... Hell Naw!)
    return QAbstractItemModel::flags(index); ///< Qt::ItemIsSelectable | Qt::ItemIsEnabled
}

QVariant SamaelItemModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ ) const
{
    if (!index.isValid())
        return QVariant();

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    SamaelImage image = qvariant_cast<SamaelImage>(node->data(index.column()));
#pragma WARNING(TODO: Aw... Hell Naw!)
    switch (role)
    {
    case Qt::DisplayRole:
        return node->data(index.column());
        break;
    case Qt::ToolTipRole:
        return QVariant();
        break;
    case Qt::AccessibleTextRole:
        return QVariant();
        break;
    case Qt::AccessibleDescriptionRole:
        return QVariant();
        break;
    default:
        return QVariant();
        break;
    }
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
        node->insertChild(i, new TreeNode(node));

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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Search
///////////////////////////////////////////////////////////////////////////////////////////////////

QModelIndexList SamaelItemModel::match(const QModelIndex &start, int role, const QVariant &value, int hits /*= 1*/, Qt::MatchFlags flags /*= Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap ) */) const
{
    QModelIndexList result;

    uint paramMatchType = flags & 0x0F;
    Qt::CaseSensitivity paramCaseSensitvity = flags & Qt::MatchCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    bool paramMatchRecursive = flags & Qt::MatchRecursive;
    bool paramMatchWrap = flags & Qt::MatchWrap;
    bool paramAllHits = (hits == -1);

    QModelIndex p = parent(start);
    int from = start.row();
    int to = rowCount(p);

    QString text; ///< only convert to a string if it is needed

    for (int i = 0; (!paramMatchWrap && i < 1) || (paramMatchWrap && i < 2); ++i) ///< iterates twice if wrapping and once if not
    {
        for (int r = from; (r < to) && (paramAllHits || result.count() < hits); ++r) ///< takes care to meet the hit count
        {
            QModelIndex idx = index(r, start.column(), p);

            if (!idx.isValid())
                continue;

            QVariant v = data(idx, role);

            if (paramMatchType == Qt::MatchExactly) ///< variant based matching
            { 
                if (value == v)
                    result.append(idx);
            } 
            else ///< string based matching
            {
                if (text.isEmpty()) ///< lazy conversion
                    text = value.toString();

                QString t = v.toString();

                switch (paramMatchType) ///< string based matching by match type
                {
                case Qt::MatchRegExp:
                    if (QRegExp(text, paramCaseSensitvity).exactMatch(t))
                        result.append(idx);
                    break;
                case Qt::MatchWildcard:
                    if (QRegExp(text, paramCaseSensitvity, QRegExp::Wildcard).exactMatch(t))
                        result.append(idx);
                    break;
                case Qt::MatchStartsWith:
                    if (t.startsWith(text, paramCaseSensitvity))
                        result.append(idx);
                    break;
                case Qt::MatchEndsWith:
                    if (t.endsWith(text, paramCaseSensitvity))
                        result.append(idx);
                    break;
                case Qt::MatchFixedString:
                    if (t.compare(text, paramCaseSensitvity) == 0)
                        result.append(idx);
                    break;
                case Qt::MatchContains:
                default:
                    if (t.contains(text, paramCaseSensitvity))
                        result.append(idx);
                }
            }

            if (paramMatchRecursive && hasChildren(idx)) ///< search the hierarchy
            {
                result += match \
                ( \
                    index( 0, idx.column(), idx ), \
                    role, \
                    ( text.isEmpty() ? value : text ), \
                    ( paramAllHits ? -1 : hits - result.count() ), \
                    flags \
                );
            }
        }
        
        from = 0; ///< prepare for the next iteration
        to = start.row();
    }

    return result;
}
