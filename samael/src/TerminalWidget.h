#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include "SamaelDockWidget.h"
class Terminal; 
class SamaelHighlighter;

class TerminalWidget : public SamaelDockWidget
{
    Q_OBJECT

public:
    TerminalWidget(QWidget *parent);
    ~TerminalWidget();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Terminal* m_Terminal;
    QVBoxLayout* m_Layout;
    SamaelHighlighter* m_Highlighter;

public slots:
    void result(QString result); ///< displays the result of a command within the terminal
    void append(QString text);
    void log(QString message);
};

#endif // TERMINALWIDGET_H
