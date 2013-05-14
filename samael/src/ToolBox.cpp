#include "stdafx.h"
#include "ToolBox.h"
#include "SamaelItemModel.h"
#include "Logger.h"
#include "SamaelImage.h"

Toolbox::Toolbox(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("Toolbox"), QStringLiteral("Operations"))
{
    // create the data model
    m_SamaelItemModel = new SamaelItemModel(m_ContentWidget);

    // configure the toolbox
    m_toolbox = new QToolBox(m_ContentWidget);

	m_toolbox->setFixedSize(250, 150);

	//m_toolbox->addItem(new QWidget(), tr("Compute"));
	//m_toolbox->addItem(new QWidget(), tr("Analyze"));

	/*buttonGroup = new QButtonGroup(m_Toolbox);
	bbb = new QPushButton();
	buttonGroup->addButton(bbb, 1);
    QPushButton *button1 = new QPushButton(buttonGroup, "Button 1");
    QPushButton *button2 = new QPushButton(buttonGroup, "Button 2");*/
    //m_toolboxTabs->addItem(buttonGroup, "Category A");

	m_toolbox->addItem(new QPushButton("Key Points"), "Compute");
	m_toolbox->addItem(new QPushButton("Visualize"), "Analyze");
	m_toolbox->addItem(new QWidget(), tr("Test"));

	/*m_Toolbox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_Toolbox->setFeatures( QDockWidget::DockWidgetClosable |
                        QDockWidget::DockWidgetFloatable |
                        QDockWidget::DockWidgetMovable);
	m_toolboxTabs = new QTabWidget(m_Toolbox);
    m_toolboxTabs->addTab(new QWidget(),"Compute");*/

    m_Layout = new QVBoxLayout(m_ContentWidget);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_toolbox);
    finalise(m_Layout);

    QLOG_INFO() << "Toolbox - Ready!";
}

Toolbox::~Toolbox()
{

}

void Toolbox::load(QString file, const QModelIndex &parent)
{
    if (file.isEmpty())
        return;

    QFileInfo info = QFileInfo(file);

    // print some general information
    QLOG_INFO() << QString("NAME: %1 [SUFFIX: %2] - BYTES: %3")
        .arg(info.fileName())
        .arg(info.suffix())
        .arg(info.size())
        .toStdString().c_str();
    QLOG_INFO() << QString("PATH: %1")
        .arg(info.absolutePath())
        .toStdString().c_str();
    QLOG_INFO() << QString("READ: %1 - WRITE: %2\n")
        .arg(info.isReadable())
        .arg(info.isWritable())
        .toStdString().c_str();

    // do the loading
    QVector<QVariant> data;
    //data.append(QVariant::fromValue(SamaelNodeMetadata()));
    //data.append(QVariant::fromValue(SamaelImage()));
    //m_SamaelItemModel->insertRow(parent.row(),data,parent);
}

void Toolbox::load(QStringList files, const QModelIndex &parent)
{
    if (files.isEmpty()) return;

    for (QStringList::const_iterator iter = files.cbegin(); iter != files.cend(); ++iter)
    {
        load(*iter, parent);
    }
}

void Toolbox::createActions()
{
   
}
