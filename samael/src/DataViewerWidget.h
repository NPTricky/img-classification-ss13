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

private:
	std::vector<cv::Mat> m_confusionMatrices;//confusion Matrices of all Classes

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
	QComboBox *m_MatrixComboBox;

    // Actions
    QAction *m_CopyAction;

    void createActions();
    void onSelectionChanged(int row, int column, int flags);

private slots:
    void displayMatrix(int i = 0);

public slots:
    void onRowChanged(int i);
    void onColumnChanged(int i);
    void onRowClicked(int i);
    void onColumnClicked(int i);
	void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

    void copyToClipboard();

	void displayMatrix(cv::Mat matrix);
	void setConfusionMatrixHeaderData(std::vector<std::string> &classNames);

	void saveConfusionMatrices();
    void loadConfusionMatrices();
};

#endif // DATAWIDGET_H
