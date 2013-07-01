#include "stdafx.h"
#include "OpenCVMatrixModel.h"

OpenCVMatrixModel::OpenCVMatrixModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_Mat(cv::Mat::eye(16, 16, CV_32F))
{

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
        return Qt::AlignCenter;
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

void OpenCVMatrixModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder */)
{
    cv::Mat result;
    cv::sort(m_Mat.col(column), result, CV_SORT_EVERY_COLUMN + CV_SORT_DESCENDING);
    result.copyTo(m_Mat.col(column));
    emit dataChanged(index(0,column),index(m_Mat.rows - 1, column));
}
