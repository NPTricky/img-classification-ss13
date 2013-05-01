#include "stdafx.h"
#include "TerminalWidget.h"
#include "Logger.h"

TerminalWidget::TerminalWidget(QWidget *parent)
    : SamaelDockWidget(parent, QStringLiteral("TerminalWidget"), QStringLiteral("Terminal"))
    , m_UserPrompt(QString("[root@samael /]# "))
    , m_InputLock(false)
    , m_HistorySkip(false)
{
    // clear history
    m_HistoryUp.clear();
    m_HistoryDown.clear();

    // configure the text edit
    m_TextEdit = new QPlainTextEdit(parent);
    m_TextEdit->setFont(QFont("Courier",9));
    m_TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_TextEdit->insertPlainText(m_UserPrompt);
    
    // configure the layout of this widget
    m_Layout = new QVBoxLayout(m_ContentWidget);
    m_Layout->setContentsMargins(0,0,0,0);
    m_Layout->addWidget(m_TextEdit);
    finalise(m_Layout);

    QLOG_INFO() << "TerminalWidget - Ready!";
}

TerminalWidget::~TerminalWidget()
{

}

/** Filter all key events. The keys are filtered and handled manually
* in order to create a typical shell-like behaviour. For example
* Up and Down arrows don't move the cursor, but allow the user to
* browse the last commands that there launched.
*/

void TerminalWidget::keyPressEvent(QKeyEvent *event)
{
    // locked State: a command has been submitted but no result
    // has been received yet.
    if(m_InputLock) return;

    switch(event->key())
    {
    case Qt::Key_Return:
        onEnter();
        break;
    case Qt::Key_Backspace:
        onLeft(event);
        break;
    case Qt::Key_Up:
        onUp();
        break;
    case Qt::Key_Down:
        onDown();
        break;
    case Qt::Key_Left:
        onLeft(event);
        break;
    case Qt::Key_Home:
        onHome();
        break;
    default:
        //((QObject*)(m_TextEdit))->event(event);
        //QPlainTextEdit::keyPressEvent(event);
        break;
    }
}

// Enter key pressed
void TerminalWidget::onEnter()
{
    QString cmd = getCommand();

    if (0 < cmd.length()) 
    {
        while (m_HistoryDown.count() > 0) 
        {
            m_HistoryUp.push(m_HistoryDown.pop());
        }

        m_HistoryUp.push(cmd);
    }

    moveToEndOfLine();

    if (cmd.length() > 0)
    {
        m_InputLock = true;
        m_TextEdit->setFocus();
        m_TextEdit->insertPlainText("\n");
        emit command(cmd);
    } else {
        m_TextEdit->insertPlainText("\n");
        m_TextEdit->insertPlainText(m_UserPrompt);
        m_TextEdit->ensureCursorVisible();
    }
}

// Result received
void TerminalWidget::result(QString result)
{
    m_TextEdit->insertPlainText(result);
    m_TextEdit->insertPlainText("\n");
    m_TextEdit->insertPlainText(m_UserPrompt);
    m_TextEdit->ensureCursorVisible();
    m_InputLock = false;
}

#pragma WARNING("log() method would need to save the current line (if it isn't a log), delete the current line, print the log and bring back the saved line for clean visuals. for now its ok.")
// same as result but without unlocking
void TerminalWidget::log(QString message)
{   
    //auto cmd = getCommand();
    //clearLine();
    m_TextEdit->appendPlainText(message);
    m_TextEdit->ensureCursorVisible();
}

// Append line but do not display prompt afterwards
void TerminalWidget::append(QString text)
{
    m_TextEdit->insertPlainText(text);
    m_TextEdit->insertPlainText("\n");
    m_TextEdit->ensureCursorVisible();
}

// Arrow up pressed
void TerminalWidget::onUp()
{
    if(0 < m_HistoryUp.count())
    {
        QString cmd = m_HistoryUp.pop();
        m_HistoryDown.push(cmd);

        clearLine();
        m_TextEdit->insertPlainText(cmd);
    }

    m_HistorySkip = true;
}

// Arrow down pressed
void TerminalWidget::onDown()
{
    if(0 < m_HistoryDown.count() && m_HistorySkip)
    {
        m_HistoryUp.push(m_HistoryDown.pop());
        m_HistorySkip = false;
    }

    if(0 < m_HistoryDown.count())
    {
        QString cmd = m_HistoryDown.pop();
        m_HistoryUp.push(cmd);

        clearLine();
        m_TextEdit->insertPlainText(cmd);
    } else {
        clearLine();
    }
}

void TerminalWidget::clearLine()
{
    QTextCursor c = m_TextEdit->textCursor();
    c.select(QTextCursor::LineUnderCursor);
    c.removeSelectedText();
    m_TextEdit->insertPlainText(m_UserPrompt);
}

// Select and return the user-input (exclude the prompt)
QString TerminalWidget::getCommand() const
{
    QTextCursor c = m_TextEdit->textCursor();
    c.select(QTextCursor::LineUnderCursor);

    QString text = c.selectedText();
    text.remove(0, m_UserPrompt.length());

    return text;
}

void TerminalWidget::moveToEndOfLine()
{
    m_TextEdit->moveCursor(QTextCursor::EndOfLine);
}

// The text cursor is not allowed to move beyond the
// prompt
void TerminalWidget::onLeft(QKeyEvent *event)
{
    if(getIndex(m_TextEdit->textCursor()) > m_UserPrompt.length())
    {
        //((QObject*)(m_TextEdit))->event(event);
        //QPlainTextEdit::keyPressEvent(event);
    }
}

// Home (pos1) key pressed
void TerminalWidget::onHome()
{
    QTextCursor c = m_TextEdit->textCursor();
    c.movePosition(QTextCursor::StartOfLine);
    c.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, m_UserPrompt.length());
    m_TextEdit->setTextCursor(c);
}

// Solution for getting x and y position of the cursor. Found
// them in the Qt mailing list
int TerminalWidget::getIndex (const QTextCursor &crQTextCursor )
{
    QTextBlock b;
    int column = 1;
    b = crQTextCursor.block();
    column = crQTextCursor.position() - b.position();
    return column;
}

void TerminalWidget::setPrompt(const QString &prompt)
{
    m_UserPrompt = prompt;
    clearLine();
}

QString TerminalWidget::prompt() const
{
    return m_UserPrompt;
}
