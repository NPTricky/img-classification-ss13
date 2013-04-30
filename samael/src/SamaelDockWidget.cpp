#include "stdafx.h"
#include "SamaelDockWidget.h"

SamaelDockWidget::SamaelDockWidget(QWidget *parent, const QString& name, const QString& title)
    : QDockWidget(parent)
{
    rename(name, title);

    m_ContentWidget = new QWidget();
    m_ContentWidget->setObjectName(QString("%1Content").arg(name).toStdString().c_str());
}

SamaelDockWidget::~SamaelDockWidget()
{

}

void SamaelDockWidget::rename(const QString& name, const QString& title)
{
    setObjectName(name);
    setWindowTitle(title.toStdString().c_str());
}

void SamaelDockWidget::finalise(QLayout* layout)
{
    m_ContentWidget->setLayout(layout);
    setWidget(m_ContentWidget);
}