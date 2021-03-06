#include "stdafx.h"
#include "CentralWidget.h"
#include "ViewerWidget.h"
#include "DataViewerWidget.h"
#include "SamaelMainWindow.h"
#include "Logger.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    // Viewer Widget
    m_ViewerWidget = new ViewerWidget(this);

    // Data Viewer Widget
    m_DataViewerWidget = new DataViewerWidget(this);

    // Layout
    m_Layout = new QStackedLayout(this);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_ViewerWidget);
    m_Layout->addWidget(m_DataViewerWidget);
    m_Layout->setCurrentIndex(1);

    this->setLayout(m_Layout);

    QLOG_INFO() << "CentralWidget - Ready!";
}

CentralWidget::~CentralWidget()
{

}

void CentralWidget::toggleWidget()
{
    int result = (m_Layout->currentIndex() + 1) % m_Layout->count();
    m_Layout->setCurrentIndex(result);
}

DataViewerWidget* CentralWidget::getDataViewerWidget()
{
	return m_DataViewerWidget;
}