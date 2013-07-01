#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>

class QTableView;
class QGridLayout;
class OpenCVMatrixModel;

class DataViewerWidget : public QWidget
{
    Q_OBJECT

public:
    DataViewerWidget(QWidget *parent);
    ~DataViewerWidget();

    void displayMatrix(cv::Mat &matrix);

private:
    QGridLayout *m_Layout;
    QTableView *m_TableView;
    OpenCVMatrixModel *m_Model;

	// Toolbar
	QToolBar *m_ToolBar;
	QLabel *m_RowLabel;
	QSpinBox *m_RowSpinBox;
	QLabel *m_ColumnLabel;
	QSpinBox *m_ColumnSpinBox;
    QLabel *m_SortLabel;
    QCheckBox *m_SortCheckBox;

    void onSelectionChanged(int row, int column, int flags);

public slots:
    void onRowChanged(int i);
    void onColumnChanged(int i);
    void onRowClicked(int i);
    void onColumnClicked(int i);
};

#endif // DATAWIDGET_H
