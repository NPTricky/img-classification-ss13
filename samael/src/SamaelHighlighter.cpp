#include "stdafx.h"
#include "SamaelHighlighter.h"

SamaelHighlighter::SamaelHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
    , m_CurrentFormatContext(CONTEXT_NONE)
{
    m_Formats.clear();
    createFormats();
}

void SamaelHighlighter::setRules(const QList<Rule>& rules)
{
    m_Rules = rules;
}

void SamaelHighlighter::setMultiLineCommentStart(const QString& string)
{
    m_MultiLineCommentStart = string;
}

void SamaelHighlighter::setMultiLineCommentEnd(const QString& string)
{
    m_MultiLineCommentEnd = string;
}

void SamaelHighlighter::createFormats()
{
    m_Formats.clear();
    m_Formats.resize(FormatType_Max);

    QTextCharFormat format;

    auto colorLightGray = QColor(qRgb(186,200,198));
    auto colorMediumGray = QColor(qRgb(130,142,140));
    auto colorDarkGray = QColor(qRgb(28,32,36));
    auto colorLightPink = QColor(qRgb(180,140,180));
    auto colorPink = QColor(qRgb(190,100,190));
    auto colorBurntOrange = QColor(qRgb(200,90,60));
    auto colorYellow = QColor(qRgb(236,202,20));
    auto colorLightGreen = QColor(qRgb(160,190,90));
    auto colorBurntGreen = QColor(qRgb(90,110,50));
    auto colorLightBrown = QColor(qRgb(140,130,110));
    auto colorBlue = QColor(qRgb(74,158,214));

    //format.setForeground(Qt::blue);
    //format.setBackground(Qt::blue);
    //format.setFontWeight(QFont::Bold);
    //format.setFontCapitalization(QFont::AllUppercase);

    format.setForeground(colorBlue);
    format.setFontWeight(QFont::Bold);
    m_Formats[Keyword] = format;
    format.setFontWeight(QFont::Normal);

    format.setForeground(colorBurntGreen);
    m_Formats[Comment] = format;

    format.setForeground(colorYellow);
    m_Formats[DataType] = format;

    format.setForeground(colorPink);
    m_Formats[BuiltInVariable] = format;

    format.setForeground(colorBurntOrange);
    m_Formats[BuiltInFunction] = format;

    format.setForeground(colorLightGray);
    m_Formats[Number] = format;

    format.setForeground(colorLightBrown);
    m_Formats[String] = format;

    format.setForeground(colorLightGreen);
    m_Formats[Misc] = format;
}

void SamaelHighlighter::highlightBlock(const QString& text)
{
    foreach (Rule rule, m_Rules)
    {
        int index = text.indexOf(rule.pattern);
        while (index >= 0)
        {
            int length = rule.pattern.matchedLength();
            setFormat(index, length, m_Formats[rule.type]);
            index = text.indexOf(rule.pattern, index + length);
        }
    }

    if (!m_MultiLineCommentStart.isEmpty())
    {
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
        {
            startIndex = text.indexOf(m_MultiLineCommentStart);
        }

        while (startIndex >= 0) 
        {
            int endIndex = text.indexOf(m_MultiLineCommentEnd, startIndex);
            int commentLength;
            if (endIndex == -1) 
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } 
            else 
            {
                commentLength = endIndex - startIndex + m_MultiLineCommentStart.length();
            }
            setFormat(startIndex, commentLength, m_Formats[Comment]);
            startIndex = text.indexOf(m_MultiLineCommentStart, startIndex + commentLength);
        }
    }
}

void SamaelHighlighter::setContext( FormatContext context )
{
    if (context == m_CurrentFormatContext)
        return;

    switch(context)
    {
    case CONTEXT_TERMINAL:
        setRules(highlightingRulesTerminal());
        setMultiLineCommentStart( "/*" );
        setMultiLineCommentEnd( "*/" );
        break;
    default:
        setRules(highlightingRulesNone());
        break;
    }
}

QList<SamaelHighlighter::Rule> SamaelHighlighter::highlightingRulesNone()
{
    return QList<SamaelHighlighter::Rule>();
}

QList<SamaelHighlighter::Rule> SamaelHighlighter::highlightingRulesTerminal()
{
    QList<SamaelHighlighter::Rule> rules;
    SamaelHighlighter::Rule rule;

    rule.type = SamaelHighlighter::Keyword;
    rule.pattern = QRegExp(
        "\\b("
        "if|else|for|while|do|struct|break|continue|discard|return|technique|pass|sampler_state|compile|true|false|packed|typedef"
        ")\\b");
    rules.append(rule);

    rule.type = SamaelHighlighter::DataType;
    rule.pattern = QRegExp(
        "\\b("
        "void|float|float[1-4]|float[1-4]x[1-4]|int|int[1-4]|int[1-4]x[1-4]|bool|bool[1-4]|bool[1-4]x[1-4]|"
        "half|half[1-4]|half[1-4]x[1-4]|fixed|fixed[1-4]|fixed[1-4]x[1-4]|"
        "sampler[1-3]D|samplerCUBE|samplerRECT|texture|string|uniform|varying|static|const|in|out|inout"
        ")\\b");
    rules.append(rule);

    rule.type = SamaelHighlighter::BuiltInVariable;
    rule.pattern = QRegExp(
        "\\b("
        "World(View)?(Projection)?(I)?(T)?|"
        "World(View)?(Projection)?(Inverse)?(Transpose)?|"
        "Model(View)?(Projection)?(I)?(T)?|"
        "Model(View)?(Projection)?(Inverse)?(Transpose)?|"
        "View(Projection)?(I)?(T)?|"
        "View(Projection)?(Inverse)?(Transpose)?|"
        "Projection(I)?(T)?|"
        "Projection(Inverse)?(Transpose)?|"
        "Time|ViewportSize|MinFilter|MagFilter|WrapS|WrapT|WrapR|BorderColor|"
        "POSITION|COLOR[0-1]?|TEXCOORD[0-7]?|NORMAL|FACE|"
        "VertexProgram|GeometryProgram|TessellationControlProgram|TessellationEvaluationProgram|FragmentProgram|"
        "DepthTestEnable|CullFaceEnable|register\\(c[1-2]+\\)"
        ")\\b");
    rules.append(rule);

    rule.type = SamaelHighlighter::BuiltInFunction;
    rule.pattern = QRegExp(
        "\\b("
        "abs|acos|all|any|asin|atan|atan2|ceil|clamp|cos|cosh|cross|degrees|determinant|dot|floor|length|lerp|"
        "max|min|mul|normalize|radians|reflect|refract|round|saturate|sin|sinh|tan|tanh|transpose|"
        "tex1D(proj)?|tex2D(proj)?|texRECT(proj)?|tex3D(proj)?|texCUBE(proj)?|"
        "offsettex2D|offsettexRECT|offsettex2DScaleBias|offsettexRECTScaleBias|tex1D_dp3|tex2D_dp3x2|"
        "texRECT_dp3x2|tex3D_dp3x3|texCUBE_dp3x3|texCUBE_reflect_dp3x3|texCUBE_reflect_eye_dp3x3|tex_dp3x2_depth|"
        "(un)?pack_4(u)?byte|(un)?pack_2ushort|(un)?pack_2half"
        ")\\b");
    rules.append(rule);

    rule.type = SamaelHighlighter::Number;
    rule.pattern = QRegExp("\\b[-+]?\\d*\\.?\\d+([eE][-+]?\\d+)?[fFhHxX]?\\b");
    rules.append(rule);

    rule.type = SamaelHighlighter::String;
    rule.pattern = QRegExp("\".*\"");
    rules.append(rule);

    rule.type = SamaelHighlighter::Misc;
    rule.pattern = QRegExp("#.*(//|$)");
    rules.append(rule);

    rule.type = SamaelHighlighter::Comment;
    rule.pattern = QRegExp("//.*$");
    rules.append(rule);

    return rules;
}

// ### Reference ### QShaderEdit
// 
//QList<SamaelHighlighter::Rule> SamaelHighlighter::highlightingRulesCgFX()
//{
//    QList<SamaelHighlighter::Rule> rules;
//    SamaelHighlighter::Rule rule;
//
//    rule.type = SamaelHighlighter::Keyword;
//    rule.pattern = QRegExp(
//        "\\b("
//        "if|else|for|while|do|struct|break|continue|discard|return|technique|pass|sampler_state|compile|true|false|packed|typedef"
//        ")\\b");
//    rules.append(rule);
//
//    rule.type = SamaelHighlighter::DataType;
//    rule.pattern = QRegExp(
//        "\\b("
//        "void|float|float[1-4]|float[1-4]x[1-4]|int|int[1-4]|int[1-4]x[1-4]|bool|bool[1-4]|bool[1-4]x[1-4]|"
//        "half|half[1-4]|half[1-4]x[1-4]|fixed|fixed[1-4]|fixed[1-4]x[1-4]|"
//        "sampler[1-3]D|samplerCUBE|samplerRECT|texture|string|uniform|varying|static|const|in|out|inout"
//        ")\\b");
//    rules.append(rule);
//
//    rule.type = SamaelHighlighter::BuiltInVariable;
//    rule.pattern = QRegExp(
//        "\\b("
//        "World(View)?(Projection)?(I)?(T)?|"
//        "World(View)?(Projection)?(Inverse)?(Transpose)?|"
//        "Model(View)?(Projection)?(I)?(T)?|"
//        "Model(View)?(Projection)?(Inverse)?(Transpose)?|"
//        "View(Projection)?(I)?(T)?|"
//        "View(Projection)?(Inverse)?(Transpose)?|"
//        "Projection(I)?(T)?|"
//        "Projection(Inverse)?(Transpose)?|"
//        "Time|ViewportSize|MinFilter|MagFilter|WrapS|WrapT|WrapR|BorderColor|"
//        "POSITION|COLOR[0-1]?|TEXCOORD[0-7]?|NORMAL|FACE|"
//        "VertexProgram|GeometryProgram|TessellationControlProgram|TessellationEvaluationProgram|FragmentProgram|"
//        "DepthTestEnable|CullFaceEnable|register\\(c[1-2]+\\)"
//        ")\\b");
//    rules.append(rule);
//
//    rule.type = SamaelHighlighter::BuiltInFunction;
//    rule.pattern = QRegExp(
//        "\\b("
//        "abs|acos|all|any|asin|atan|atan2|ceil|clamp|cos|cosh|cross|degrees|determinant|dot|floor|length|lerp|"
//        "max|min|mul|normalize|radians|reflect|refract|round|saturate|sin|sinh|tan|tanh|transpose|"
//        "tex1D(proj)?|tex2D(proj)?|texRECT(proj)?|tex3D(proj)?|texCUBE(proj)?|"
//        "offsettex2D|offsettexRECT|offsettex2DScaleBias|offsettexRECTScaleBias|tex1D_dp3|tex2D_dp3x2|"
//        "texRECT_dp3x2|tex3D_dp3x3|texCUBE_dp3x3|texCUBE_reflect_dp3x3|texCUBE_reflect_eye_dp3x3|tex_dp3x2_depth|"
//        "(un)?pack_4(u)?byte|(un)?pack_2ushort|(un)?pack_2half"
//        ")\\b");
//    rules.append(rule);
//
//    rule.type = SamaelHighlighter::Number;
//    rule.pattern = QRegExp("\\b[-+]?\\d*\\.?\\d+([eE][-+]?\\d+)?[fFhHxX]?\\b");
//    rules.append(rule);
//
//    rule.type = SamaelHighlighter::String;
//    rule.pattern = QRegExp("\".*\"");
//    rules.append(rule);
//
//    rule.type = SamaelHighlighter::Misc;
//    rule.pattern = QRegExp("#.*(//|$)");
//    rules.append(rule);
//
//    rule.type = SamaelHighlighter::Comment;
//    rule.pattern = QRegExp("//.*$");
//    rules.append(rule);
//
//    return rules;
//}
