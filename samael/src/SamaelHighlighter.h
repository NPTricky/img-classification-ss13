#ifndef TERMINALHIGHLIGHTER_H
#define TERMINALHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SamaelHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum FormatType 
    {
        Keyword = 0,
        Comment,
        DataType,
        BuiltInVariable,
        BuiltInFunction,
        Number,
        String,
        Misc,
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
    void setMultiLineCommentStart(const QString& string);
    void setMultiLineCommentEnd(const QString& string);

private:
    QList<Rule> m_Rules;
    QString m_MultiLineCommentStart;
    QString m_MultiLineCommentEnd;
    FormatContext m_CurrentFormatContext;

    QVector<QTextCharFormat> m_Formats;
    void createFormats();
};

#endif // TERMINALHIGHLIGHTER_H
