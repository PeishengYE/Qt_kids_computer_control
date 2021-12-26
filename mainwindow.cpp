#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);


    ui->statusBar->showMessage(tr("Waiting..."));
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 8005)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }else{
        qDebug() << "Server has started. Listening to port 8005.";
    }

    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::sendFortune);



}
void MainWindow::readFortune()
{
    qDebug()<< "readFortune()>> " ;
        QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString data = QString(client->readAll());
    qDebug()<< "readFortune()>> "<< data ;
//    QMessageBox::warning(this, tr("Warning"), data);
    ui->warning->setText(data);
client->disconnectFromHost();
//    in.startTransaction();

//        QString nextFortune;
//        in >> nextFortune;
//        qDebug()<< "readFortune()>> "<< nextFortune ;
//        if (!in.commitTransaction()){
//            qDebug()<< "readFortune()>> done" ;
//             return;
//        }



}

void MainWindow::sendFortune()
{
//! [5]
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];
//! [4] //! [7]

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QIODevice::readyRead, this, &MainWindow::readFortune);
    in.setDevice(clientConnection);
       in.setVersion(QDataStream::Qt_4_0);

//! [7] //! [8]

    clientConnection->write(block);
    //clientConnection->disconnectFromHost();
//! [5]
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::exitApp()
{
  QCoreApplication::quit();

}

void MainWindow::warningMesg(QString message)
{

    qDebug()<< "Slot warningMesg() with: " << message;
//    QMessageBox::warning(this, tr("Warning"), message);
    ui->statusBar->showMessage(message);
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
