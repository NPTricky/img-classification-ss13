#include "stdafx.h"
#include "DataViewerWidget.h"
#include <QTableView>
#include <QGridLayout>
#include <QComboBox>
#include <QModelIndex>
#include "OpenCVMatrixModel.h"
#include "SamaelApplication.h"

DataViewerWidget::DataViewerWidget(QWidget *parent)
    : QWidget(parent)
{
    // Model
    m_Model = new OpenCVMatrixModel(this);

	// Toolbar
	m_ToolBar = new QToolBar(this);
    
    createActions();

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
    m_SortCheckBox = new QCheckBox(tr("DO NOT USE"),m_ToolBar);
    m_SortCheckBox->setToolTip(tr("Sort Column By Selection"));
	m_MatrixComboBox = new QComboBox(m_ToolBar);

	m_ToolBar->addWidget(m_RowLabel);
	m_ToolBar->addWidget(m_RowSpinBox);
	m_ToolBar->addWidget(m_ColumnLabel);
	m_ToolBar->addWidget(m_ColumnSpinBox);
    m_ToolBar->addWidget(m_SortLabel);
    m_ToolBar->addWidget(m_SortCheckBox);
    m_ToolBar->addAction(m_CopyAction);
	m_ToolBar->addWidget(m_MatrixComboBox);

    // View
    m_TableView = new QTableView(this);
    m_TableView->setModel(m_Model);
    cv::Mat matrix = cv::Mat::eye(101, 101, CV_32F);
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
	connect(m_TableView->selectionModel(),SIGNAL(currentChanged(const QModelIndex,const QModelIndex)),this,SLOT(onCurrentChanged(const QModelIndex,const QModelIndex)));

    this->setLayout(m_Layout);
}

DataViewerWidget::~DataViewerWidget()
{

}

void DataViewerWidget::displayMatrix(cv::Mat& matrix)
{
	m_confusionMatrices.push_back(matrix);

    if (!matrix.empty())
        m_Model->setSourceMatrix(matrix);

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
	int state = Qt::CheckState(m_SortCheckBox->checkState());

    if (state == 2) // checked
    {
		m_TableView->verticalHeader()->setSortIndicator(i, Qt::AscendingOrder);
        m_TableView->sortByColumn(i);
        return;
    }

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

void DataViewerWidget::copyToClipboard()
{
    if (m_TableView->selectionModel()->selectedIndexes().size() <= 1)
        m_TableView->selectAll();

    QItemSelectionModel * selection = m_TableView->selectionModel();
    QModelIndexList indices = selection->selectedIndexes();
    int columnCount = indices.last().column() - indices.first().column();
    int headerWidth = 16;

    // QModelIndex::operator < sorts first by row, then by column.
    // this is what we need
    //std::sort(indices.begin(), indices.end());
    std::sort(indices.begin(),indices.end());

    // You need a pair of indexes to find the row changes
    QModelIndex previous = indices.takeFirst();

    // create the latex column alignment string
    QString columnAlignment;
    for (int i = 0; i <= columnCount + 1; i++) // +1 for row header
    {
        columnAlignment.append(" c");
    }
    columnAlignment.append(" ");

    QString selected_text;
    QModelIndex current;

    // add first elements
    selected_text.append("\\scalebox{0.1}{\n");
    selected_text.append("\\begin{tabular}{" + columnAlignment + "}\n");
    //selected_text.append("\\hline\n");

    // header rows beginning
    selected_text.append(QString("%1 & ").arg("",headerWidth));

    // header rows
    int i = previous.column();
    for (i; i < indices.last().column(); i++)
    {
            selected_text.append(QString("%1 & ").arg(m_TableView->model()->headerData(i,Qt::Vertical).toString()));
    }
    selected_text.append(QString("%1 \\\\ \\hline \n").arg(m_TableView->model()->headerData(i,Qt::Vertical).toString()));

    // first row beginning
    selected_text.append(QString("%1 &").arg(m_TableView->model()->headerData(previous.row(),Qt::Vertical).toString(),headerWidth));

    Q_FOREACH(current, indices)
    {
        // At this point `text` contains the text in one cell

        // If you are at the start of the row the row number of the previous index
        // isn't the same.  Text is followed by a row separator, which is a newline.
        if (current.row() != previous.row())
        {
            selected_text.append(QString("%1 \\\\ \\hline \n").arg(m_TableView->model()->data(previous).toString(),4));
            selected_text.append(QString("%1 &").arg(m_TableView->model()->headerData(current.row(),Qt::Vertical).toString(),headerWidth));
        }
        else
        {
            selected_text.append(QString("%1 &").arg(m_TableView->model()->data(previous).toString(),4));
        }

        // Otherwise it's the same row, so append a column separator, which is a tab.
        previous = current;
    }

    // add last elements
    selected_text.append(QString("%1 \\\\\n").arg(m_TableView->model()->data(current).toString(),4));
    selected_text.append("\\hline\n");
    selected_text.append("\\end{tabular}\n");
    selected_text.append("}");

    GetApp()->clipboard()->setText(selected_text);
}

void DataViewerWidget::createActions()
{
    // "Copy To Clipboard" Action
    m_CopyAction = new QAction(tr("&Copy To Clipboard"), this);
    m_CopyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    m_CopyAction->setToolTip(tr("Copy To Clipboard (LaTeX Tabular)"));
    m_CopyAction->setStatusTip(tr("Copy To Clipboard (LaTeX Tabular)"));
    connect(m_CopyAction, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

    QIcon iconClipboard;
    iconClipboard.addFile(":/content/icons/dataviewerwidget_clipboard.svg", QSize(), QIcon::Normal, QIcon::Off);
    m_CopyAction->setIcon(iconClipboard);
}

void DataViewerWidget::setConfusionMatrixHeaderData(std::vector<std::string> &classNames)
{
    m_Model->setConfusionMatrixHeaderData(classNames);
}

void DataViewerWidget::saveConfusionMatrices()
{
  cv::FileStorage file("confusionMatrix.xml", cv::FileStorage::WRITE);
  
  int runNumber = m_confusionMatrices.size();//number of runs
  file << "run_number" << runNumber;

  std::string confusionMatrixName = "Confusion_Matrix";
  for(int i = 0; i < runNumber; i++)
  {
    std::ostringstream converter;
    converter << i;
    file << (confusionMatrixName + converter.str()).c_str() << m_confusionMatrices[i];
  }

  int classNumber = m_Model->rowCount();//number of classes
  file << "class_number" << classNumber;

  std::string fieldName = "FieldName";
  for(int i = 0; i < classNumber; i++)
  {
    std::ostringstream converter;
    converter << i;
    file << (fieldName + converter.str()).c_str() << m_Model->headerData(i, Qt::Vertical).toString().toStdString().c_str();
  }

  file.release();
}

void DataViewerWidget::loadConfusionMatrices()
{
  cv::FileStorage file("confusionMatrix.xml", cv::FileStorage::READ);

  int runNumber;
  file["run_number"] >> runNumber;

  std::string confusionMatrixName = "Confusion_Matrix";
  for(int i = 0; i < runNumber; i++)
  {
    std::ostringstream converter;
    converter << i;
    file[(confusionMatrixName + converter.str()).c_str()] >> m_confusionMatrices[i];
  }

  int classNumber;
  file["class_number"] >> classNumber;

  std::string className;
  std::string fieldName = "FieldName";
  for(int i = 0; i < classNumber; i++)
  {
    std::ostringstream converter;
    converter << i;
    file[(fieldName + converter.str()).c_str()] >> className;
    m_Model->setHeaderData(i, QString(className.c_str()));
  }

  file.release();
}

void DataViewerWidget::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	m_RowSpinBox->blockSignals(true);
	m_ColumnSpinBox->blockSignals(true);
	onRowClicked(current.row());
	onColumnClicked(current.column());
	m_RowSpinBox->blockSignals(false);
	m_ColumnSpinBox->blockSignals(false);
}