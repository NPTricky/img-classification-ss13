#ifndef SAMAELTREEITEMMODEL_H
#define SAMAELTREEITEMMODEL_H

#include <QAbstractItemModel>

class TreeNode;

class SamaelItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    SamaelItemModel(QObject *parent = nullptr);
    ~SamaelItemModel();

    /// basic model
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex( )) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /// editability of items & datastructure
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, const QVector<QVariant>& data, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    /// search
    virtual QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;

private:
    TreeNode* m_RootNode;
};

#endif // SAMAELTREEITEMMODEL_H
