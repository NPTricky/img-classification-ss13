#include "stdafx.h"
#include "DataViewerWidget.h"
#include <QTableView>
#include <QGridLayout>
#include "OpenCVMatrixModel.h"

DataViewerWidget::DataViewerWidget(QWidget *parent)
    : QWidget(parent)
{
    // Model
    m_Model = new OpenCVMatrixModel(this);

    // Widgets
    m_TableView = new QTableView(this);
    m_TableView->setModel(m_Model);
    displayMatrix(cv::Mat());

    // Layout
    m_Layout = new QGridLayout(this);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_TableView);

    this->setLayout(m_Layout);
}

DataViewerWidget::~DataViewerWidget()
{

}

void DataViewerWidget::displayMatrix(cv::Mat& matrix)
{
    if (!matrix.empty())
        m_Model->setSourceMatrix(matrix);

    m_TableView->resizeColumnsToContents();
    m_TableView->resizeRowsToContents();
}
