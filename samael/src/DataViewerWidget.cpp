#include "stdafx.h"
#include "DataViewerWidget.h"
#include <QTableView>
#include <QGridLayout>

DataViewerWidget::DataViewerWidget(QWidget *parent)
    : QWidget(parent)
{
    // Widgets
    m_TableView = new QTableView(this);

    // Layout
    m_Layout = new QGridLayout(this);
    m_Layout->addWidget(m_TableView);

    this->setLayout(m_Layout);
}

DataViewerWidget::~DataViewerWidget()
{

}
