#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QVector>
#include <QApplication>
#include <QMessageBox>
#include "highlighter.h"
#include "udef.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns;
    bool flag = getKeyword(keywordPatterns);  //the config be loaded only in system initialization at a times
#if _debug_getKeyword_ == _on
    qDebug() << keywordPatterns;
    qDebug() << keywordPatterns.size();
#endif

    if (flag == false)      // if the file don't be open, then execute the default pattern if the macro is on
    {
#if (_KEYWORD == C_SET)
    keywordPatterns << "\\bvoid\\b" << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                    << "\\bshort\\b" << "\\blong\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                    << "\\bstruct\\b" << "\\bunion\\b" << "\\benum\\b" << "\\btypedef\\b" << "\\bsizeof\\b"
                    << "\\bstatic\\b" << "\\bauto\\b" << "\\bregister\\b" << "\\bextern\\b" << "\\bconst\\b" << "\\bvolatile\\b"
                    << "\\breturn\\b" << "\\bcontinue\\b" << "\\bbreak\\b" << "\\bgoto\\b"
                    << "\\bif\\b" << "\\belse\\b" << "\\bswitch\\b" << "\\bcase\\b" << "\\bdefault\\b"
                    << "\\bfor\\b" << "\\bdo\\b" << "\\bwhile\\b";
//                    << "\\binclude\\b" << "\\bdefine\\b" << "\\bdefined\\b" << "\\bpragma\\b"
//                    << "\\bif\\b" << "\\bifdef\\b" << "\\bifndef\\b"
//                    << "\\belse\\b" << "\\belif\\b"
//                    << "\\bendif\\b";
#elif (_KEYWORD == CPP_SET)
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b";
                    //<< "\\bQHP\\b";
#endif
    }

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);                 // for Qt class
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);                     // in this position can avoid the condition // main()
    functionFormat.setForeground((Qt::blue));
    rule.pattern = QRegExp("\\b[A-Za-z_]+[A-Za-z0-9_]*(?=\\()");
//    rule.pattern = QRegExp("\\b[A-Za-z0-9]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    stdFileIncludeFormat.setForeground(Qt::darkGreen);          //#include <stdio.h>
    rule.pattern = QRegExp("<.*>");
    rule.format = stdFileIncludeFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen);       //Qt::red
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);      //Qt::red
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

//    functionFormat.setFontItalic(true);
//    functionFormat.setForeground((Qt::blue));
//    rule.pattern = QRegExp("\\b[A-Za-z_]+[A-Za-z0-9_]*(?=\\()");
//    rule.pattern = QRegExp("\\b[A-Za-z0-9]+(?=\\()");       //when 00main() error
//    rule.format = functionFormat;
//    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

bool Highlighter::getKeyword(QStringList &keywordList)
{
    QFile file(":/config/keyword");
#if _debug_getKeyword_ == _on
    qDebug() << file.exists();
#endif
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
//        QMessageBox::warning(this,
//             tr("Linuxfor Innovator"),          // some wrong at tr function
//             tr("Cannot read the file %1:\n%2.")
//             .arg(":/config/keyword")
//             .arg(file.error()));
#if _debug_getKeyword_ == _on
        qDebug() << "failure to open the file!";
#endif
        return false;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QString text = in.readAll();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

//    QVector<Keyword> keywords;
//    Keyword support;

    QRegExp rx;
    rx.setPattern("(\\[.*\\])");
    rx.setMinimal(true);        //not greedy
    int pos = rx.indexIn(text);
    bool key = false;                           // to limit that read the first data into support structure
    while(pos != -1)
    {
        QRegExp exp("\\b(\\w+)\\b");            //keywordset=something, something MUST be a character or string
        QString string = rx.cap(1);
#if _debug_getKeyword_ == _on
        qDebug() << "string: " <<string;
#endif
        Keyword kw;                     //
        exp.setMinimal(true);
        int idx = exp.indexIn(string);
        if(key == false)
        {
            while(idx != -1)
            {
                support.keywordset << exp.cap(1);
#if _debug_getKeyword_ == _on
                qDebug() << "keyword: " << exp.cap(1);
#endif
                idx += exp.matchedLength();
                idx = exp.indexIn(string, idx);
            }
            support.language = "distinct";
            key = true;
        }
        else
        {
            while(idx != -1)
            {
                kw.keywordset << exp.cap(1);
#if _debug_getKeyword_ == _on
                qDebug() << "keyword: " << exp.cap(1);
#endif
                idx += exp.matchedLength();
                idx = exp.indexIn(string, idx);
            }
            kw.language = "restrict";
            keywords.append(kw);
        }

        int length = rx.matchedLength();
        pos = rx.indexIn(text, pos + length);
    }

    QVector<Keyword>::iterator i;
    for(i = keywords.begin(); i != keywords.end(); ++i)
    {
        QString string = i->keywordset.at(1);
#if _debug_getKeyword_ == _on
        qDebug() << "------------------------------";
        qDebug() << "string: " << string;
        qDebug() << "------------------------------";
#endif
        if(support.keywordset.contains(string, Qt::CaseSensitive))
        {
            i->language = i->keywordset.at(1);
            i->keywordset.removeFirst();
            i->keywordset.removeFirst();
#if _debug_getKeyword_ == _on
            qDebug() << "here";
            qDebug() << "keywordset: " << i->keywordset;
            qDebug() << "size: " << i->keywordset.size();
            qDebug() << "language: " << i->language;
#endif
        }
    }

#if _debug_getKeyword_ == _on
    qDebug() << "=============================<<<";
    foreach (const Keyword kw, keywords) {
        if(kw.language != "restrict")
        {
            qDebug() << "keywordset: " << kw.keywordset;
            qDebug() << "size: " << kw.keywordset.size();
            qDebug() << "language: " << kw.language;
        }
    }
    qDebug() << "=============================<<<";
#endif

    foreach (const Keyword kw, keywords)
    {
        if (kw.language != "restrict")
        {
            for(int i = 0; i < kw.keywordset.size(); ++i)
            {
                if(keywordList.contains(kw.keywordset.at(i), Qt::CaseSensitive))
                    continue;
                keywordList.append(kw.keywordset.at(i));
            }
#if _debug_getKeyword_ == _on
            qDebug() << "-----------------------------";
            qDebug() << "keywordset: " << kw.keywordset;
            qDebug() << "size: " << kw.keywordset.size();
            qDebug() << "language: " << kw.language;
#endif
        }
    }

    file.close();
    return true;
}

