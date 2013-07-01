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

	// Toolbar
	m_ToolBar = new QToolBar(this);

    int margin = 6;
	m_RowLabel = new QLabel(tr("ROW"),m_ToolBar);
    m_RowLabel->setContentsMargins(0,0,margin,0);
	m_RowSpinBox = new QSpinBox(m_ToolBar);
	m_ColumnLabel = new QLabel(tr("COLUMN"),m_ToolBar);
    m_ColumnLabel->setContentsMargins(margin,0,margin,0);
	m_ColumnSpinBox = new QSpinBox(m_ToolBar);

	m_ToolBar->addWidget(m_RowLabel);
	m_ToolBar->addWidget(m_RowSpinBox);
	m_ToolBar->addWidget(m_ColumnLabel);
	m_ToolBar->addWidget(m_ColumnSpinBox);

    // Widgets
    m_TableView = new QTableView(this);
    m_TableView->setModel(m_Model);
    displayMatrix(cv::Mat());

    // Layout
    m_Layout = new QGridLayout(this);
    m_Layout->setContentsMargins(0,0,0,0);
	m_Layout->addWidget(m_ToolBar,0,0);
    m_Layout->addWidget(m_TableView,1,0);

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
