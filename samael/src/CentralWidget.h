#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
class QStackedLayout;
class ViewerWidget;
class DataViewerWidget;

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    CentralWidget(QWidget *parent);
    ~CentralWidget();

private:
    QStackedLayout *m_Layout;
    ViewerWidget *m_ViewerWidget;
    DataViewerWidget *m_DataViewerWidget;

public slots:
    void toggleWidget();
    void displayMatrix(cv::Mat &matrix);
    void setConfusionMatrixHeaderData(std::vector<std::string> &classNames);
};

#endif // CENTRALWIDGET_H
