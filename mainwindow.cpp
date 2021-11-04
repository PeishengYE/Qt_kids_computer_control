#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

//    setAttribute(Qt::WA_TranslucentBackground);

//    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
//    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
//setWindowFlags(Qt::WindowStaysOnTopHint)

    CheckLogThread * thread = new CheckLogThread();
    thread->start();

    connect(thread, SIGNAL(sendCounts(int, QString)), this, SLOT(patchingProgress(int, QString)));
    connect(thread, SIGNAL(done()), this, SLOT(patchingFinished()));
    connect(thread, SIGNAL(warningMesg(QString)), this, SLOT(warningMesg(QString)));
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::warningMesg(QString err)
{


    QMessageBox::warning(this, tr("Error"), err);

}






void MainWindow::patchingProgress( int count, QString line)
{
    qDebug()<< "Slot patchingProgress() with: " << "count: " << count << "Line: " << line;
    qint64 percentage = 100 * count / 1000;
    ui->patchProgress->setValue((int) percentage);
    ui->statusBar->showMessage(tr("%1").arg(line));
}

void MainWindow::patchingFinished()
{
QCoreApplication::instance()->quit();
}
