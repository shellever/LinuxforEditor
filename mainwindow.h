/*
    Copyright (C) 2015 Linuxfor
    Contact: linuxfor@163.com
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QUrl>
#include "codeeditor.h"
#include "highlighter.h"
#include "calculator.h"

#include "udef.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent * event);
    void dragEnterEvent(QDragEnterEvent *event);//event which is sent to widget when a drag and drop actin enters it
    void dropEvent(QDropEvent *event);          //event which is sent when a drag and drop action is completed

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void openRecentFile();
//    void close();         //the same as newFile()

    void build();
    void run();

//    void option();
    void calc();
//    void tetrix();
//    void musicPlayer();
//    void ascii();

//    void guide();
    void about();
//    void aboutQt();

    void documentWasModified();
    void timerHandle();

private:
    void setupEditor();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createTimer();

    void readSettings();
    void writeSettings();

    void updateRecentFileActions();

    bool maybeSave();

    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    QString getFileName(const QString &fullFileName);
    QString getFilePath(const QString &fullFileName);
    QString getFileSuffix(const QString &fullFileName);
    QString getFileCompleteBaseName(const QString &fullFileName);

    void preprocessor();

private:
    QTimer *timer;                  //Timer
    QLabel *url;                    //statusbar for display some url, such as hao123.com ans so on

  //  QPlainTextEdit *textEdit;
    CodeEditor *editor;             //CodeEditor
    Highlighter *highlighter;       //Highlighter
    Calculator *calculator;         //Calcalator
//    TetrixWindow *window;           //Tetrix game
//    VideoPlayer *player;            //video player
    QString curFile;
    QString tmpFile;

    QMenu *fileMenu;
    QMenu *recentFileMenu;
    QMenu *editMenu;
    QMenu *buildMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *buildToolBar;
//    QToolBar *toolsToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    enum { MaxRecentFiles = 8 };
    QAction *recentFileActs[MaxRecentFiles];

    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *selectAllAct;

    QAction *buildAct;
    QAction *runAct;

    QAction *asciiAct;          //ascii table
    QAction *calcAct;           //calculator
    QAction *tetrixAct;         //tetrix game
    QAction *musicPlayerAct;    //music player
    QAction *optionAct;         //option

    QAction *aboutQtAct;
    QAction *aboutAct;
    QAction *guideAct;

};

#endif // MAINWINDOW_H
