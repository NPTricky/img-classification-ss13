#ifndef TERMINALHIGHLIGHTER_H
#define TERMINALHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SamaelHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum FormatType 
    {
        Trace = 0,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        Date,
        Time,
        Source,
        FormatType_Max
    };

    enum FormatContext
    {
        CONTEXT_NONE,
        CONTEXT_TERMINAL
    };

    struct Rule
    {
        QRegExp pattern;
        FormatType type;
    };

    SamaelHighlighter(QTextDocument* parent);

    void setContext(FormatContext);

protected:
    void highlightBlock(const QString& text);
    QList<SamaelHighlighter::Rule> highlightingRulesNone();
    QList<SamaelHighlighter::Rule> highlightingRulesTerminal();

    void setRules(const QList<Rule>& rules);

private:
    QList<Rule> m_Rules;
    FormatContext m_CurrentFormatContext;

    QVector<QTextCharFormat> m_Formats;
    void createFormats();
};

#endif // TERMINALHIGHLIGHTER_H
