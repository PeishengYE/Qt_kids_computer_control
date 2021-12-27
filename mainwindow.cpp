#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    QTimer *timer = new QTimer(this);

     connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

     timer->start(1000);


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
    connect(this, SIGNAL(messageArrived(QString)), this, SLOT(warningMesg(QString)));


}
void MainWindow::readFortune()
{
    qDebug()<< "readFortune()>> " ;
        QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString data = QString(client->readAll());
    qDebug()<< "readFortune()>> "<< data ;

    ui->warning->setText(data);
    client->disconnectFromHost();
   emit messageArrived(data);
    qDebug()<< "readFortune()>> byebye" ;
}

void MainWindow::updateTimer()
{
  if(!stopCountdownTimer){
      initSeconds --;
      if(initSeconds== 0) stopCountdownTimer = true;
      QString time = QDateTime::fromTime_t(initSeconds).toUTC().toString("hh:mm:ss");
      ui->timer->setText(time);
  }
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
    ui->statusBar->showMessage(message);
//    QMessageBox::warning(nullptr, tr("Warning"), message);
    QMessageBox* msgBox = new QMessageBox( this );
       msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
       msgBox->setStandardButtons( QMessageBox::Ok );
       msgBox->setWindowTitle( tr("Warning") );
       msgBox->setText( message );
       msgBox->setModal( false ); // if you want it non-modal
       msgBox->open( this, SLOT(msgBoxClosed(QAbstractButton*)) );

}








