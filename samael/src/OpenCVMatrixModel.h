#ifndef OPENCVMATRIXMODEL_H
#define OPENCVMATRIXMODEL_H

#include <QAbstractTableModel>

class OpenCVMatrixModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    OpenCVMatrixModel(QObject *parent);
    ~OpenCVMatrixModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void setHeaderData(int section, const QVariant &value, int role = Qt::EditRole);
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    void setConfusionMatrixHeaderData(std::vector<std::string> &classNames);

private:
    cv::Mat m_Mat;
    QList<QVariant> m_VerticalHeader;
    QList<QVariant> m_HorizontalHeader;

    void clearHeader();

public slots:
    void setSourceMatrix(cv::Mat &mat);
};

#endif // OPENCVMATRIXMODEL_H
