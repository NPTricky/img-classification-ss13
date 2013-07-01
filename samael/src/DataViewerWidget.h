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
    QGridLayout *m_Layout;
    QTableView *m_TableView;
    OpenCVMatrixModel *m_Model;

public slots:
    void displayMatrix(cv::Mat& matrix);
};

#endif // DATAWIDGET_H
