#include "stdafx.h"
#include "OpenCVMatrixModel.h"

OpenCVMatrixModel::OpenCVMatrixModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_Mat = cv::Mat::eye(16, 16, CV_32F);
}

OpenCVMatrixModel::~OpenCVMatrixModel()
{

}

int OpenCVMatrixModel::rowCount(const QModelIndex &parent /*= QModelIndex( ) */) const
{
    return m_Mat.rows;
}

int OpenCVMatrixModel::columnCount(const QModelIndex &parent /*= QModelIndex( ) */) const
{
    return m_Mat.cols;
}

QVariant OpenCVMatrixModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole */) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        return m_Mat.at<float>(index.row(),index.column());
    case Qt::TextAlignmentRole:
        return Qt::AlignHCenter;
    default:
        return QVariant();
    }
}

void OpenCVMatrixModel::setSourceMatrix(cv::Mat &mat)
{
    beginResetModel();
    m_Mat = mat;
    endResetModel();
}
