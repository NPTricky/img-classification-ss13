#ifndef TERMINAL_H
#define TERMINAL_H

#include <QPlainTextEdit>

class Terminal : public QPlainTextEdit
{
    Q_OBJECT

public:
    Terminal(QWidget *parent);
    ~Terminal();

    QString prompt() const;
    void setPrompt(const QString &prompt);

    void result(QString result); ///< displays the result of a command within the terminal
    void append(QString text);
    void log(QString message);

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

signals:
    void command(QString command); ///< fired on user input
};

#endif // TERMINAL_H
