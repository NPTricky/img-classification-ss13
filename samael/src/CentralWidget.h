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
};

#endif // CENTRALWIDGET_H
