#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    Server * server = new Server(this);
    server->start();
    ui->statusBar->showMessage(tr("Waiting patching process ..."));
    connect(server->getInternalQTcpServer(), SIGNAL(receviedMesg(QString )), this, SLOT(warningMesg(QString )));
    m_timerId = startTimer(1000);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::exitApp()
{
  QCoreApplication::quit();

}

void MainWindow::warningMesg(QString err)
{
      quitAppTimer->start(10000);

    QMessageBox::warning(this, tr("Warning"), err);
    ui->statusBar->showMessage(err);
}






void MainWindow::patchingProgress( int count, QString line)
{
    qDebug()<< "Slot patchingProgress() with: " << "count: " << count << "Line: " << line;
    qint64 percentage = 100 * count / 2000;

    ui->statusBar->showMessage(tr("[%1]::%2").arg(count).arg(line));
}

void MainWindow::patchingFinished()
{
QCoreApplication::instance()->quit();
}
