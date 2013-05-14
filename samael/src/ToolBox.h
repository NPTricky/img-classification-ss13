#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "SamaelDockWidget.h"
class SamaelItemModel;
class QToolBox;

class Toolbox : public SamaelDockWidget
{
    Q_OBJECT

public:
    Toolbox(QWidget *parent);
    ~Toolbox();

private:
    void load(QString file, const QModelIndex &parent = QModelIndex());
    void load(QStringList files, const QModelIndex &parent = QModelIndex());
    void createActions();

	// Container
    QToolBox *m_toolbox;
	QVBoxLayout* m_Layout;

    SamaelItemModel* m_SamaelItemModel;

};

#endif // TOOLBOX_H
