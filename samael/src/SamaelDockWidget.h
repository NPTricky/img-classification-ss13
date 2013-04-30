#ifndef SAMAELDOCKWIDGET_H
#define SAMAELDOCKWIDGET_H

#include <QDockWidget>

class SamaelDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    SamaelDockWidget(QWidget *parent, const QString& name, const QString& title);
    virtual ~SamaelDockWidget();

    void rename(const QString& name, const QString& title);

protected:
    QWidget m_ContentWidget;

private:
    
};

#endif // SAMAELDOCKWIDGET_H
