//#include <QMessageBox>
//#include <QTextStream>
//#include <QApplication>
//#include "parser.h"

//Parser::Parser(QString fname, QWidget *parent)
//    : QWidget(parent)
//{
//    parse(getFileText(fname));
//    process();
//}

//QStringList Parser::getSupport()
//{
//    return keywords.at(0).keywordset;
//}

//int Parser::getSupportCount()
//{
//    return keywords.at(0).amount;
//}

//QString Parser::getSupportStatus()
//{
//    return keywords.at(0).language;     //on = distinct     off = restrict
//}

//QStringList Parser::getKeywordSet(QString item)
//{
//    foreach(const Keyword kw, keywords)
//    {
//        if(kw.language == item)
//            return kw.keywordset;
//    }
//}

//int Parser::getKeywordSetCount(QString item)
//{
//    foreach (const Keyword kw, keywords)
//    {
//        if(kw.language == item)
//            return kw.amount;
//    }
//}

//QString Parser::getFileText(QString fname)
//{
//    QFile file(fname);
//    if(!file.open(QFile::ReadOnly | QFile::Text))
//    {
//        QMessageBox::warning(this, tr("Linuxfor Innovator"),
//                             tr("Cannot read the file %1:\n%2.")
//                             .arg(fname)
//                             .arg(file.error()));
//#if _debug_process_ == _on
//        qDebug() << "failure to open the file!";
//#endif
//        return NULL;
//    }
//    QTextStream in(&file);
//#ifndef QT_NO_CURSOR
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//#endif
//    QString text = in.readAll();
//#ifndef QT_NO_CURSOR
//    QApplication::restoreOverrideCursor();
//#endif
//    return text;
//}

//void Parser::parse(QString text)
//{
//    QRegExp rx("(\\[.*\\])");
//    rx.setMinimal(true);                //not greedy
//    int pos = rx.indexIn(text);
//    while(pos != -1)
//    {
//        QRegExp exp("\\b(\\w+)\\b");    //just any word
//        exp.setMinimal(true);
//        QString item = rx.cap(1);       //item [...]
//        int idx;
//        Keyword kw;
//        int amount = 0;
//        idx = rx.indexIn(item);
//        while(idx != -1)
//        {
//            kw.keywordset << exp.cap(1);
//            kw.amount = ++amount;
//            idx += exp.matchedLength();
//            idx = exp.indexIn(item, idx);
//        }
//        kw.language = "restrict";
//        keywords.append(kw);

//        pos += rx.matchedLength();
//        pos = rx.indexIn(text, pos);
//    }
//}

//void Parser::process()
//{
//    QVector<Keyword>::iterator i;
//    i = keywords.begin();
//    QString status = i->keywordset.at(1);
//#if _debug_process_ == _on
//    qDebug() << "------------------------------";
//    qDebug() << "status: " << status;
//    qDebug() << "------------------------------";
//#endif
//    if(status == "off")
//        return;
//    i->amount -= 2;
//    i->language = "distinct";
//    i->keywordset.removeFirst();
//    i->keywordset.removeFirst();
//    QStringList support = i->keywordset;
//    i++;
//    while(i++ != keywords.end())
//    {
//        QString string = i->keywordset.at(1);
//#if _debug_process_ == _on
//        qDebug() << "------------------------------";
//        qDebug() << "string: " << string;
//        qDebug() << "------------------------------";
//#endif
//        if(support.contains(string, Qt::CaseSensitive))
//        {
//#if _debug_process_ == _on
//            qDebug() << "before>>";
//            qDebug() << "keywordset: " << i->keywordset;
//            qDebug() << "amount: " << i->amount;
//            qDebug() << "language: " << i->language;
//#endif
//            i->amount -= 2;
//            i->language = i->keywordset.at(1);
//            i->keywordset.removeFirst();
//            i->keywordset.removeFirst();
//#if _debug_process_ == _on
//            qDebug() << "after>>";
//            qDebug() << "keywordset: " << i->keywordset;
//            qDebug() << "amount: " << i->amount;
//            qDebug() << "language: " << i->language;
//#endif
//        }
//    }
//}
