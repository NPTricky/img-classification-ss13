#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include "SamaelDockWidget.h"
class Terminal; 

class TerminalWidget : public SamaelDockWidget
{
    Q_OBJECT

public:
    TerminalWidget(QWidget *parent);
    ~TerminalWidget();

protected:
    void keyPressEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent*) { /* Ignore */ }
    void mouseDoubleClickEvent(QMouseEvent*) { /* Ignore */ }
    void mouseMoveEvent(QMouseEvent*) { /* Ignore */ }
    void mouseReleaseEvent(QMouseEvent*) { /* Ignore */ }

private:
    Terminal* m_Terminal;
    QVBoxLayout* m_Layout;

public slots:
    void result(QString result); ///< displays the result of a command within the terminal
    void append(QString text);
    void log(QString message);
};

#endif // TERMINALWIDGET_H
