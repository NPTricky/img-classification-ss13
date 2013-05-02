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

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /// editability of items & datastructure
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
    TreeNode* m_RootNode;
};

#endif // SAMAELTREEITEMMODEL_H
