#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

#define C_SET       0
#define CPP_SET     1
#define _KEYWORD C_SET

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

struct Keyword
{
    QString language;           //
    QStringList keywordset;     //
};

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0); 

protected:
    void highlightBlock(const QString &text);

private:
    bool getKeyword(QStringList &keywordList);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat stdFileIncludeFormat;

//    struct Keyword
//    {
//        QString language;           //
//        QStringList keywordset;     //
//    };
    QVector<Keyword> keywords;
    Keyword support;
};

#endif // HIGHLIGHTER_H
