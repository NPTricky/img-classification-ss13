#include "stdafx.h"
#include "OpenCVMatrixModel.h"

OpenCVMatrixModel::OpenCVMatrixModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_Mat(cv::Mat::eye(101, 101, CV_32F))
    , m_HorizontalHeader(QList<QVariant>())
    , m_VerticalHeader(QList<QVariant>())
{
    clearHeader();
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
    clearHeader();
    endResetModel();
}

void OpenCVMatrixModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder */)
{
    cv::Mat result;
    cv::sort(m_Mat.col(column), result, CV_SORT_EVERY_COLUMN + CV_SORT_DESCENDING);
    result.copyTo(m_Mat.col(column));
    emit dataChanged(index(0,column),index(m_Mat.rows - 1, column));
}

QVariant OpenCVMatrixModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (orientation)
    {
    case Qt::Horizontal:
        return m_HorizontalHeader.at(section);
    case Qt::Vertical:
        return m_VerticalHeader.at(section);
    default:
        return QVariant();
    }
}

void OpenCVMatrixModel::setHeaderData(int section, const QVariant &value, int role /*= Qt::EditRole */)
{
    setHeaderData(section, Qt::Horizontal, value, role);
    setHeaderData(section, Qt::Vertical, value, role);
}

bool OpenCVMatrixModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role /*= Qt::EditRole */)
{
    if (role != Qt::EditRole)
        return false;

    switch (orientation)
    {
    case Qt::Horizontal:
        m_HorizontalHeader[section] = value;
        return true;
    case Qt::Vertical:
        m_VerticalHeader[section] = value;
        return true;
    default:
        return false;
    }
}

void OpenCVMatrixModel::setConfusionMatrixHeaderData(std::vector<std::string> &classNames)
{
    clearHeader();

    if ((classNames.size() > m_Mat.rows) || (classNames.size() > m_Mat.cols))
        return;

    for (int i = 0; i < classNames.size(); i++)
    {
        setHeaderData(i,QString::fromStdString(classNames[i]));
    }
}

void OpenCVMatrixModel::clearHeader()
{
    m_HorizontalHeader.clear();
    for (int i = 0; i <= m_Mat.cols; i++)
    {
        m_HorizontalHeader << QVariant(i+1);
    }

    m_VerticalHeader.clear();
    for (int j = 0; j <= m_Mat.rows; j++)
    {
        m_VerticalHeader << QVariant(j+1);
    }
}
