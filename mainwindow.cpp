#include <QtWidgets>
//#include <QTime>
#include "mainwindow.h"
#include "udef.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//    textEdit = new QPlainTextEdit;
//    //Courier Serif
//    QFont editFont("Courier", 12, QFont::Light);
//    textEdit->setFont(editFont);


    setupEditor();

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createTimer();

   // writeSettings();
    readSettings();

    connect(editor->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

    editor->setAcceptDrops(false);          //most important to archieve the drag and drop
    setAcceptDrops(true);                   //most important to archieve the drag and drop

    setCentralWidget(editor);
//    connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

//    setCentralWidget(textEdit);

    //setWindowTitle(tr("Linuxfor Innovator for QHP"));
    setWindowIcon(QIcon(":/images/logo_5.png"));
    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFilePath(QString());
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
    {
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();      //Sets the drop action to be the proposed action
    else
        event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls())                 //Returns true if the object can return a list of urls; otherwise returns false
    {
        QList<QUrl> urlList = mimeData->urls();
        QString filename = urlList.at(0).toLocalFile();
#if (_debug_dropEvent_ == _on)
        qDebug() << "filename: " << filename;
#endif
        if(!filename.isEmpty())
        {
            loadFile(filename);
        }
    }
}

void MainWindow::newFile()
{
    if(maybeSave())
    {
        //textEdit->clear();
        editor->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
            "/",        //default path to open"/Linuxfor/qhp/",
            tr("Linuxfor Innovator (*.qhp);;"
               "Text Files (*.txt);;"
               "C Files (*.c *h);;"
               "C++ Files (*.cpp *.h);;"
               "Python Files (*.py);;"
               "Perl Files (*.pl);;"
               "XML Files (*.xml);;"
               "All Files (*.*)"));
        //QMessageBox::information(this, "For Debug Information", fileName);//just for debug  //path/filename.qhp
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if(curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}

bool MainWindow::saveAs()
{
//    QFileDialog dialog(this);
//    dialog.setWindowModality(Qt::WindowModal);
//    dialog.setAcceptMode(QFileDialog::AcceptSave);
//    dialog.exec();
//    QStringList files = dialog.selectedFiles();     //

//    if(files.isEmpty())
//        return false;

//    return saveFile(files.at(0));                   //single file to handle


    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
        "/"+getFileName(tmpFile),
        tr("Linuxfor Innovator (*.qhp);;"
           "Text Files (*.txt);;"
           "C Files (*.c *h);;"
           "C++ Files (*.cpp *.h);;"
           "Python Files (*.py);;"
           "Perl Files (*.pl);;"
           "XML Files (*.xml);;"
           "All Files (*.*)"));
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::build()
{
    preprocessor();         //generate a execute file using gcc compiler
}

void MainWindow::run()
{
    QString cmd;
    cmd = tr("%1.exe").arg(curFile);
    cmd.replace("/", "\\");
    system(cmd.toStdString().data());
}

void MainWindow::calc()
{
    calculator = new Calculator();        //if it doesn't take to a new, just like a flash
    calculator->setAttribute(Qt::WA_DeleteOnClose);
    calculator->show();
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About Linuxfor Innnovator"));
    msgBox.setWindowIcon(QIcon(":/images/logo_2.png"));
    msgBox.setIconPixmap(QPixmap(":/images/logo_2.png"));
    //msgBox.setText(tr("The statement for Linuxfor Innovator for QHP: "));
    msgBox.setInformativeText(tr("<p>The statement for Linuxfor Innovator for QHP: </p>"
                                 "<p>The <b>Linuxfor Editor</b> is so far unique editor to "
                                 "program a new program language, that is <b>QHP</b> language, "
                                 "created by <b>Shell eva</b>. This editor will give you a hand to "
                                 "go to program through QHP language.</p>"));
    msgBox.setStandardButtons(QMessageBox::Close);
    //msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.exec();

//    QMessageBox::about(this, tr("About Linuxfor Editor"),
//            tr("The <b>Linuxfor Editor</b> is so far unique editor to "
//               "program a new program language, that is <b>QHP</b> language, "
//               "created by <b>Shell eva</b>. This editor will give you a hand to "
//               "go to program through QHP language."));

//    QMessageBox::information(this, tr("About Linuxfor Editor"),
//             tr("The <b>Linuxfor Editor</b> is so far unique editor to "
//                "program a new program language, that is <b>QHP</b> language, "
//                "created by Shell eva. This editor will give you a hand to "
    //                "go to program through QHP language."));
}

void MainWindow::documentWasModified()
{
    setWindowModified(editor->document()->isModified());
    //    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::timerHandle()
{
    static int counter = 0;
    switch(counter)
    {
    case 0:
        url->setText(tr("<a href = \"www.linuxfor@163.com\">Linuxfor Innovator for QHP</a>"));
        break;
    case 1:
        url->setText(tr("<a href = \"www.linuxfor@163.com\">Copyright (C) 2015 Linuxfor</a>"));
        break;
    case 2:
        url->setText(tr("<a href = \"www.linuxfor@163.com\">Author: shell eva</a>"));
        break;
    case 3:
        url->setText(tr("<a href = \"www.linuxfor@163.com\">Contact: www.linuxfor@163.com</a>"));
        break;
    case 4:
        url->setText(tr("<a href = \"www.hao123.com\">MainPage: www.hao123.com</a>"));
        break;
    default:
        break;
    }
    if(++counter >= 5)
        counter = 0;
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new CodeEditor;
    editor->setFont(font);
    editor->setTabStopWidth(32);        //8px per char

    highlighter = new Highlighter(editor->document());

//    textEdit = new QPlainTextEdit;
//    textEdit->setFont(font);
//    highlighter = new Highlighter(textEdit->document());        //set up syntax highlighting
}

void MainWindow::createMenus()
{
    fileMenu = this->menuBar()->addMenu(tr("&File"));
    recentFileMenu = new QMenu(tr("&Recent Files"), fileMenu);
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addMenu(recentFileMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    for (int i = 0; i < MaxRecentFiles; ++i)
        recentFileMenu->addAction(recentFileActs[i]);
    updateRecentFileActions();

    editMenu = this->menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAct);

    //menuBar()->addSeparator();

    buildMenu = this->menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(buildAct);
    buildMenu->addAction(runAct);

    toolsMenu = this->menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(asciiAct);
    toolsMenu->addAction(calcAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(tetrixAct);
    toolsMenu->addAction(musicPlayerAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(optionAct);

    helpMenu = this->menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(guideAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAct);
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

    buildToolBar = addToolBar(tr("Build"));
    buildToolBar->addAction(buildAct);
    buildToolBar->addAction(runAct);
}

void MainWindow::createStatusBar()
{
    this->statusBar()->showMessage(tr("Ready"));
    url = new QLabel(this);
    //url->setFrameShape(QFrame::Box);
    //url->setFrameShadow(QFrame::Sunken);
    url->setFrameStyle(QFrame::Box | QFrame::Sunken);
    url->setText(tr("<a href = \"www.linuxfor@163.com\">Linuxfor Innovator for QHP</a>"));
    url->setTextFormat(Qt::RichText);
    url->setOpenExternalLinks(true);
    this->statusBar()->addPermanentWidget(url);
}

void MainWindow::createTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerHandle()));   //From then on, the timerHandle() slot is called every two seconds
    timer->start(2000);     //two seconds/2000 millisecond
}

void MainWindow::readSettings()
{
    QSettings settings("./config/settings.ini", QSettings::IniFormat);
    QPoint pos = settings.value("mainwindow/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("mainwindow/size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
    //Company: Linuxfor
    //Product: Linuxfor Innovator
//    QSettings settings("Linuxfor", "Linuxfor Innovator");
//    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
//    QSize size = settings.value("size", QSize(400, 400)).toSize();
//    resize(size);
//    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("./config/settings.ini", QSettings::IniFormat);  //be in debug directory
    settings.beginGroup("Linuxfor");
    settings.setValue("Editor", "Linuxfor Innovator for QHP");
    settings.setValue("Contact", "www.linuxfor@163.com");
    settings.setValue("Author", "shell eva");
    settings.endGroup();
    settings.beginGroup("mainwindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
//    QSettings settings("Linuxfor", "Linuxfor Innovator");
//    settings.setValue("pos", pos());
    //    settings.setValue("size", size());
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S)); //CTRL + SHIFT + S
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));               //ALT + F4
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));



    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"),this);
    cutAct->setShortcut(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(cutAct, SIGNAL(triggered()), editor, SLOT(cut()));
//    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
     connect(copyAct, SIGNAL(triggered()), editor, SLOT(copy()));
//    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAct, SIGNAL(triggered()), editor, SLOT(paste()));
//    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    selectAllAct = new QAction(tr("Se&lect All"), this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    selectAllAct->setStatusTip(tr("Select the entire contents of the file into the clipboard"));
    connect(selectAllAct, SIGNAL(triggered()), editor, SLOT(selectAll()));
//    connect(selectAllAct, SIGNAL(triggered()), textEdit, SLOT(selectAll()));



    buildAct = new QAction(QIcon(":/images/build.png"), tr("&Build"), this);
    buildAct->setShortcut(tr("Ctrl+B"));
    buildAct->setStatusTip(tr("Build the source file"));
    connect(buildAct, SIGNAL(triggered()), this, SLOT(build()));

    runAct = new QAction(QIcon(":/images/run.png"), tr("&Run"), this);
    runAct->setShortcut(tr("Ctrl+R"));
    runAct->setStatusTip(tr("Run the target"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));



    asciiAct = new QAction(tr("&Ascii"), this);
    asciiAct->setShortcut(tr("Ctrl+I"));
    asciiAct->setStatusTip(tr("Search the character from the ascii table"));
    //connect(asciiAct, SIGNAL(triggered()), this, SLOT(ascii()));

    calcAct = new QAction(QIcon(":/images/calc.png"), tr("&Calculator"), this);
    calcAct->setShortcut(tr("Ctrl+L"));
    calcAct->setStatusTip(tr("Use the calculator"));
    connect(calcAct, SIGNAL(triggered()), this, SLOT(calc()));

    tetrixAct = new QAction(tr("&Tetrix"), this);
    tetrixAct->setShortcut(tr("Ctrl+E"));
    tetrixAct->setStatusTip(tr("Enjoy the classic tetrix game..."));
    //connect(tetrixAct, SIGNAL(triggered()), this, SLOT(tetrix()));

    musicPlayerAct = new QAction(tr("&Music Player"), this);
    musicPlayerAct->setShortcut(tr("Ctrl+Y"));
    musicPlayerAct->setStatusTip(tr("Start the music player..."));
//    connect(playerAct, SIGNAL(triggered()), this, SLOT(musicPlayer()));

    optionAct = new QAction(QIcon(":/images/option_1.png"), tr("&Option"), this);
    optionAct->setShortcut(tr("Ctrl+P"));
    optionAct->setStatusTip(tr("Set up the option for Innovator"));
    //connect(optionAct, SIGNAL(triggered()), this, SLOT(option()));



    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutAct = new QAction(tr("&About Innovator"), this);
    aboutAct->setStatusTip(tr("Show the relative Linuxfor Innovator Information..."));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    guideAct = new QAction(tr("&Guide"), this);
    guideAct->setStatusTip(tr("Quick start guide for program..."));
    //connect(guideAct, SIGNAL(triggered()), this, SLOT(guide()));



    cutAct->setEnabled(false);
    connect(editor, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
//    connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));

    copyAct->setEnabled(false);
    connect(editor, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
//    connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
}

bool MainWindow::maybeSave()
{
    if(editor->document()->isModified())
//    if(textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Linuxfor Innovator"),
            tr("The document has been modified.\n"
               "Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);
        if(ret == QMessageBox::Save)
            return save();
        else if(ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Linuxfor Innovator"),
            tr("Cannot read the file %1:\n%2.")
            .arg(fileName)
            .arg(file.error()));

        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    editor->setPlainText(in.readAll());
//    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    this->statusBar()->showMessage(tr("File loaded"), 2000);        //2000ms timeout
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Linuxfor Innovator"),
            tr("Cannot write the file %1:\n%2.")
            .arg(fileName)
            .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << editor->toPlainText();
//    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    this->statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    static int num = 1;
    curFile = fileName;
    editor->document()->setModified(false);
//    textEdit->document()->setModified(false);
    setWindowModified(false);

    //QString shownName = curFile;
    QString appendName = " - Linuxfor Innovator for QHP";
#if (_debug_setCurrentFile_ == _on)
    QMessageBox::information(this, "For Debug Information", fileName);//just for debug
    QMessageBox::information(this, "For Debug Information", appendName);//just for debug
#endif
    QString shownName  = getFileName(curFile) + " [" + getFilePath(curFile) + "] " + appendName;
#if (_debug_setCurrentFile_ == _on)
    QMessageBox::information(this, "For Debug Information", shownName);//just for debug
#endif
    if(curFile.isEmpty())
    {
        tmpFile = tr("untitled%1").arg(num++);
        shownName = tmpFile + appendName;
    }
#if (_debug_setCurrentFile_ == _on)
    qDebug() << "shownName: " << shownName;
    //QMessageBox::information(this, "For Debug Information", shownName);//just for debug
#endif
    //setWindowFilePath(shownName);     //this method will cause the incomplete display in windows title
    setWindowTitle("[*]"+shownName);    //well done

    //>>START recent files settings
    setWindowFilePath(curFile);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(curFile);               //files.removeAll(fileName);
    if(!curFile.isEmpty())                  //?? remove the empty recent file
        files.prepend(curFile);                 //files.prepend(fileName);
    while (files.size() > MaxRecentFiles)   // <= MaxRecentFiles
        files.removeLast();
    settings.setValue("recentFileList", files);
    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
    //>>END recent files settings
}

QString MainWindow::getFileName(const QString &fullFileName)
{
    QString fname = QFileInfo(fullFileName).fileName();
    return fname;
}

QString MainWindow::getFilePath(const QString &fullFileName)
{
    return QFileInfo(fullFileName).absolutePath();
}

QString MainWindow::getFileSuffix(const QString &fullFileName)
{
    return QFileInfo(fullFileName).suffix();        //"/tmp/archive.tar.gz" => "gz"
}

QString MainWindow::getFileCompleteBaseName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).completeBaseName();//"/tmp/archive.tar.gz" => "archive.tar"
}

void MainWindow::preprocessor()
{
    if(editor->document()->isModified())
        save();

    QFile file(curFile);
#if _debug_preprocessor_ == _on
    qDebug() << "file.exists: " << file.exists();
#endif
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Linuxfor Innovator"),
                             tr("Cannot read the file %1:\n%2.")
                             .arg(curFile)
                             .arg(file.error()));
        return;
    }

    QFile stdfile(":/config/stddef");
#if _debug_preprocessor_ == _on
    qDebug() << "stdfile.exists: " << stdfile.exists();
#endif
    if(!stdfile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Linuxfor Innovator"),
                             tr("Cannot read the file stddef:\n%1.")
                             .arg(stdfile.error()));
        return;
    }

    QString cfname = curFile;
    qDebug() << "cfname: " << cfname;
    cfname = cfname.replace("/", "\\") + ".c";
    qDebug() << "cfname: " << cfname;
    QFile cfile(cfname.toStdString().data());  //QFile cfile("d:\\buffer\\helloworld.c");

#if _debug_preprocessor_ == _on
    qDebug() << "cfile.exists: " << cfile.exists();
#endif
    if(!cfile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "cfname created failure!";
        return;
    }

    QTextStream in(&file);
    QTextStream sin(&stdfile);

    QTextStream out(&cfile);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QString headers = "#include <stdio.h>\n";
    QString targetText = headers + sin.readAll() + in.readAll();
    out << targetText;
#if _debug_preprocessor_ == _on
    qDebug() << "target: " << targetText;
#endif
//    out << sin.readAll() + in.readAll();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    file.close();
    stdfile.close();
    cfile.close();

    QString cmd = tr("gcc -o %1.exe %2.c").arg(curFile).arg(curFile);
#if _debug_preprocessor_ == _on
    qDebug() << "cmd: " << cmd;
#endif
#ifndef __UNIX
    system(cmd.toStdString().data());       //gcc -o D:/buffer/pre.qhp.exe D:/buffer/pre.qhp.c => right

    cmd = tr("del %1.c").arg(curFile);
#if _debug_preprocessor_ == _on
    qDebug() << "cmd: " << cmd;
#endif
    cmd.replace("/", "\\");
    system(cmd.toStdString().data());       //del D:/buffer/pre.qhp.c => wrong, so take a replace before
//    QFile::remove(cfname.toStdString().data());
#endif
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    //QString fname = QFileInfo(fullFileName).fileName();
    return QFileInfo(fullFileName).fileName();
}
