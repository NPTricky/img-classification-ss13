#include "stdafx.h"
#include "SamaelDockWidget.h"

SamaelDockWidget::SamaelDockWidget(QWidget *parent, const QString& name, const QString& title)
    : QDockWidget(parent)
{
    rename(name, title);
}

SamaelDockWidget::~SamaelDockWidget()
{

}

void SamaelDockWidget::rename( const QString& name, const QString& title )
{
    setObjectName(name);
    setWindowTitle(title.toStdString().c_str());
}
