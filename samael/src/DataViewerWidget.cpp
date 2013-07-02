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
    m_RowSpinBox->setMinimum(1);
	m_ColumnLabel = new QLabel(tr("COLUMN"),m_ToolBar);
    m_ColumnLabel->setContentsMargins(margin,0,margin,0);
	m_ColumnSpinBox = new QSpinBox(m_ToolBar);
    m_ColumnSpinBox->setMinimum(1);
    m_SortLabel = new QLabel(tr("SORT"),m_ToolBar);
    m_SortLabel->setContentsMargins(margin,0,margin,0);
    m_SortCheckBox = new QCheckBox(tr("[DO NOT USE \"SORT\" OR YOU'LL DIE A SLOW AND PAINFUL DEATH!]"), m_ToolBar);
    m_SortCheckBox->setToolTip(tr("[YES, I MEAN IT. NOT IF YOUR MATRIX CONTAINS ANYTHING OF RELEVANCE!]"));

	m_ToolBar->addWidget(m_RowLabel);
	m_ToolBar->addWidget(m_RowSpinBox);
	m_ToolBar->addWidget(m_ColumnLabel);
	m_ToolBar->addWidget(m_ColumnSpinBox);
    m_ToolBar->addWidget(m_SortLabel);
    m_ToolBar->addWidget(m_SortCheckBox);

    // View
    m_TableView = new QTableView(this);
    m_TableView->setModel(m_Model);
    cv::Mat matrix = cv::Mat::eye(128, 128, CV_32F);
    displayMatrix(matrix);

    // Layout
    m_Layout = new QGridLayout(this);
    m_Layout->setContentsMargins(0,0,0,0);
	m_Layout->addWidget(m_ToolBar,0,0);
    m_Layout->addWidget(m_TableView,1,0);

    // Connections
    connect(m_RowSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onRowChanged(int)));
    connect(m_ColumnSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onColumnChanged(int)));
    connect(m_TableView->verticalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(onRowClicked(int)));
    connect(m_TableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(onColumnClicked(int)));

    this->setLayout(m_Layout);
}

DataViewerWidget::~DataViewerWidget()
{

}

void DataViewerWidget::displayMatrix(cv::Mat& matrix)
{
    if (!matrix.empty())
        m_Model->setSourceMatrix(matrix);

    //m_TableView->resizeColumnsToContents();
    //m_TableView->resizeRowsToContents();

    m_RowSpinBox->setMaximum(matrix.rows);
    m_ColumnSpinBox->setMaximum(matrix.cols);
}

void DataViewerWidget::onSelectionChanged(int row, int column, int flags)
{
    QModelIndex target = m_Model->index(row - 1, column - 1);
    m_TableView->selectionModel()->setCurrentIndex(target, QItemSelectionModel::SelectionFlags(flags));
    m_TableView->scrollTo(target, QAbstractItemView::PositionAtCenter);
}

void DataViewerWidget::onRowChanged(int i)
{
    int flags = QItemSelectionModel::SelectCurrent;
    onSelectionChanged(i, m_ColumnSpinBox->value(), flags);
}

void DataViewerWidget::onColumnChanged(int i)
{
    int flags = QItemSelectionModel::SelectCurrent;
    onSelectionChanged(m_RowSpinBox->value(), i, flags);
}

void DataViewerWidget::onRowClicked(int i)
{
    m_RowSpinBox->setValue(i + 1);
}

void DataViewerWidget::onColumnClicked(int i)
{
    int state = Qt::CheckState(m_SortCheckBox->checkState());

    if (state == 2) // checked
    {
        m_TableView->horizontalHeader()->setSortIndicator(i, Qt::AscendingOrder);
        m_TableView->sortByColumn(i);
        return;
    }

    m_ColumnSpinBox->setValue(i + 1);
}
