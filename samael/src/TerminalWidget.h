#ifndef SAMAELCONSOLEWIDGET_H
#define SAMAELCONSOLEWIDGET_H

#include <QPlainTextEdit>
#include "SamaelDockWidget.h"

class TerminalWidget : public SamaelDockWidget
{
    Q_OBJECT

public:
    TerminalWidget(QWidget *parent);
    ~TerminalWidget();

    QString prompt() const;
    void setPrompt(const QString &prompt);

protected:
    void keyPressEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent*) { /* Ignore */ }
    void mouseDoubleClickEvent(QMouseEvent*) { /* Ignore */ }
    void mouseMoveEvent(QMouseEvent*) { /* Ignore */ }
    void mouseReleaseEvent(QMouseEvent*) { /* Ignore */ }

private:
    void onLeft(QKeyEvent *event);
    void onEnter();
    void onUp();
    void onDown();
    void onHome();

    void moveToEndOfLine();
    void clearLine();
    QString getCommand() const;

    int getIndex (const QTextCursor &crQTextCursor );

    QString m_UserPrompt;
    QStack<QString> m_HistoryUp;
    QStack<QString> m_HistoryDown;
    bool m_InputLock;
    bool m_HistorySkip;
    QPlainTextEdit* m_TextEdit;
    QVBoxLayout* m_Layout;

signals:
    void command(QString command); ///< fired on user input

public slots:
    void result(QString result); ///< displays the result of a command within the terminal
    void append(QString text);
    void log(QString message);
};

#endif // SAMAELCONSOLEWIDGET_H
