#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(graphics);        //initialize the graphics resources
    Q_INIT_RESOURCE(config);          //initialize the resources about configuration
    Q_INIT_RESOURCE(translations);    //initialize the translation for character set

    QApplication app(argc, argv);
    app.setOrganizationName("Linuxfor");
    app.setApplicationName("Linuxfor Innovator for QHP");

    /*
    //Qt 5.3 for Desktop (MinGW 4.8 32 bit)
    //cd d:\MyDocument\MyDevelopment\Qt\Qt5.4.0\editor_6_0\translations
    //lupdate -verbose editor.pro       //create a *ts file
    //linguist                          //start the Qt Linguist tools
    //lrelease -verbos editor.pro       //create *.qm file
    //or file/release to create *.qm file in Qt Linguist
    QTranslator translator;
    translator.load(":/translations/multilanguage_zh.qm");
    app.installTranslator(&translator); //before the MainWindow starting
    */

    //app.setWindowIcon(QIcon(":/images/logo.png"));
    MainWindow mainWin;
    mainWin.resize(650, 500);
    mainWin.show();

    return app.exec();
}
