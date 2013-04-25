#include "stdafx.h"
#include "TerminalWidget.h"
#include "Logger.h"

TerminalWidget::TerminalWidget(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_UserPrompt(QString("[root@samael /]# "))
    , m_InputLock(false)
    , m_HistorySkip(false)
{
    setFont(QFont("Courier",9));

    clear();
    m_HistoryUp.clear();
    m_HistoryDown.clear();
    setLineWrapMode(NoWrap);
    insertPlainText(m_UserPrompt);
    
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
        QPlainTextEdit::keyPressEvent(event);
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
        setFocus();
        insertPlainText("\n");
        emit command(cmd);
    } else {
        insertPlainText("\n");
        insertPlainText(m_UserPrompt);
        ensureCursorVisible();
    }
}

// Result received
void TerminalWidget::result(QString result)
{
    insertPlainText(result);
    insertPlainText("\n");
    insertPlainText(m_UserPrompt);
    ensureCursorVisible();
    m_InputLock = false;
}

#pragma WARNING("log() method would need to save the current line (if it isn't a log), delete the current line, print the log and bring back the saved line for clean visuals. for now its ok.")
// same as result but without unlocking
void TerminalWidget::log(QString message)
{   
    //auto cmd = getCommand();
    //clearLine();
    appendPlainText(message);
    ensureCursorVisible();
}

// Append line but do not display prompt afterwards
void TerminalWidget::append(QString text)
{
    insertPlainText(text);
    insertPlainText("\n");
    ensureCursorVisible();
}

// Arrow up pressed
void TerminalWidget::onUp()
{
    if(0 < m_HistoryUp.count())
    {
        QString cmd = m_HistoryUp.pop();
        m_HistoryDown.push(cmd);

        clearLine();
        insertPlainText(cmd);
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
        insertPlainText(cmd);
    } else {
        clearLine();
    }
}

void TerminalWidget::clearLine()
{
    QTextCursor c = this->textCursor();
    c.select(QTextCursor::LineUnderCursor);
    c.removeSelectedText();
    this->insertPlainText(m_UserPrompt);
}

// Select and return the user-input (exclude the prompt)
QString TerminalWidget::getCommand() const
{
    QTextCursor c = this->textCursor();
    c.select(QTextCursor::LineUnderCursor);

    QString text = c.selectedText();
    text.remove(0, m_UserPrompt.length());

    return text;
}

void TerminalWidget::moveToEndOfLine()
{
    QPlainTextEdit::moveCursor(QTextCursor::EndOfLine);
}

// The text cursor is not allowed to move beyond the
// prompt
void TerminalWidget::onLeft(QKeyEvent *event)
{
    if(getIndex(textCursor()) > m_UserPrompt.length())
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}

// Home (pos1) key pressed
void TerminalWidget::onHome()
{
    QTextCursor c = textCursor();
    c.movePosition(QTextCursor::StartOfLine);
    c.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, m_UserPrompt.length());
    setTextCursor(c);
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
