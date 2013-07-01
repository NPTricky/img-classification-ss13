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
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

private:
    cv::Mat m_Mat;
    
public slots:
    void setSourceMatrix(cv::Mat &mat);
};

#endif // OPENCVMATRIXMODEL_H
