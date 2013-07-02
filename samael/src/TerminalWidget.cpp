#include "stdafx.h"
#include "TerminalWidget.h"
#include "Logger.h"
#include "Terminal.h"
#include "SamaelHighlighter.h"

TerminalWidget::TerminalWidget(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("TerminalWidget"), QStringLiteral("Terminal"))
{
    this->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);

    // configure the text edit
    m_Terminal = new Terminal(m_ContentWidget);
    m_Terminal->setFont(QFont("Courier",9));
    m_Terminal->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_Terminal->setMaximumBlockCount(100);

    // background & text color
    auto p = m_Terminal->palette();
    p.setColor(QPalette::Base,QColor(qRgb(28,32,36)));
    p.setColor(QPalette::Text,QColor(qRgb(186,200,198)));
    m_Terminal->setPalette(p);

    // configure the text highlighter
    m_Highlighter = new SamaelHighlighter(m_Terminal->document());
    m_Highlighter->setContext(SamaelHighlighter::CONTEXT_TERMINAL);

    // script system? anyone? the entry point is over here!
    connect(m_Terminal, SIGNAL(command(QString)), this, SLOT(result(QString)));

    // configure the layout of this widget
    m_Layout = new QVBoxLayout(m_ContentWidget);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_Terminal);
    finalise(m_Layout);

    QLOG_INFO() << "TerminalWidget - Ready!";
}

TerminalWidget::~TerminalWidget()
{

}

void TerminalWidget::keyPressEvent(QKeyEvent *event)
{
    static_cast<QObject*>(m_Terminal)->event(event);
}

void TerminalWidget::result(QString result)
{
    m_Terminal->result(result);
}

void TerminalWidget::log(QString message)
{   
    m_Terminal->log(message);
}

void TerminalWidget::append(QString text)
{
    m_Terminal->append(text);
}
