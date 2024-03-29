#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    const int tcpPort = 8005;
	ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::WindowStaysOnTopHint |  Qt::WindowMinimizeButtonHint
                   | Qt::WindowMaximizeButtonHint| Qt::Tool);
    ui->statusBar->showMessage(tr("Waiting..."));


    QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
     timer->start(1000);



    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, tcpPort)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }else{
        qDebug() << "Server has started. Listening to port " << tcpPort;
    }


    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::handleNewSocketConnection);
    connect(this, SIGNAL(messageArrived(QString)), this, SLOT(warningMesg(QString)));


}
void MainWindow::readSocket()
{
//    qDebug()<< "readSocket()>> " ;
        QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString data = QString(client->readAll());
    qDebug()<< "readSocket()>> "<< data ;


    client->disconnectFromHost();

    executeCmd(data);
//    qDebug()<< "readSocket()>> byebye" ;
}

void MainWindow::powerOffComputer()
{
    QProcess *myProcess = new QProcess(nullptr);

 #if defined(Q_OS_UNIX) || defined(Q_OS_LINUX) || defined(Q_OS_QNX)
    QString program = "sudo" ;
    QStringList arguments ;
    arguments << "shutdown" << "-P";
    QString res = runCmd(myProcess,  program,  arguments);

#elif defined(Q_OS_WIN32)
    QString program = "shutdown" ;
    QStringList arguments ;
    arguments << "/r" ;
    QString res = runCmd(myProcess,  program,  arguments);

#endif

    qDebug()<< "cmd result: "<< res;
}

void MainWindow::showMessage(QString cmdStr)
{
    QRegularExpression       rx("\\{.*\\}", QRegularExpression::MultilineOption
                                | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = rx.match(cmdStr);
                  if (match.hasMatch()) {
                      QString warning = match.captured(0);
                      warning = warning.trimmed().remove(QChar('{'), Qt::CaseInsensitive)
                              .remove(QChar('}'), Qt::CaseInsensitive);
                      qDebug()<< "cmd received "<< warning;
                       ui->warning->setText(warning);
                      emit messageArrived(warning);
                  }else{
                      qDebug()<< "no matched ";
                  }
}

void MainWindow::executeCmd(QString cmdStr)
{
    if(cmdStr.contains(MAGIC_NUM)){
        qDebug()<< "magic found!";
        if(cmdStr.contains(POWER_OFF)){
            qDebug()<< "POWER_OFF found!";
            powerOffComputer();
          }
        if(cmdStr.contains(SHOW_MESG)){
            qDebug()<< "SHOW_MESG found!";
            showMessage(cmdStr);
        }
        if(cmdStr.contains(SET_TIME_TO_POWER_OFF)){
            QRegularExpression       rx("\\{\\s*([0-9]*)\\s*\\}", QRegularExpression::MultilineOption
                                        | QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatch match = rx.match(cmdStr);
                          if (match.hasMatch()) {
                              QString secondsToPowerDown = match.captured(0);
                              secondsToPowerDown = secondsToPowerDown.trimmed().remove(QChar('{'), Qt::CaseInsensitive)
                                      .remove(QChar('}'), Qt::CaseInsensitive);
                              qDebug()<< "Seconds to Power Down: "<< secondsToPowerDown;
                              setInitSeconds(secondsToPowerDown.toInt());

                          }else{
                              qDebug()<< "no matched ";
                          }
        }


    }else{
        qDebug()<< "no Magic number! ";
    }
}

QString MainWindow::runCmd(QProcess *myProcess, QString program, QStringList& arguments)
{
    QTextStream out(stdout);
   /***********************/
//    out << "launch process..." << endl;
    myProcess->start(program, arguments);

//    out << "waiting for finishing... " << endl;
    if (!myProcess->waitForFinished()){
        out << "error on waitForFinished()." << endl;
        return "";
    }
//    out << "readAllStandardError()>> " << endl;
    QByteArray errorResult = myProcess->readAllStandardError();
    out << QString(errorResult) << endl;

//    out << "readAllStandardOutput()>> " << endl;
    QByteArray standardResult = myProcess->readAllStandardOutput();
    out << QString(standardResult) << endl;
    return QString(standardResult);
    /***********************/
}


void MainWindow::updateTimer()
{
  if(!stopCountdownTimer){
      initSeconds --;
      if(initSeconds == 0) {
           stopCountdownTimer = true;
           powerOffComputer();
      }else{
          QString time = QDateTime::fromTime_t(initSeconds).toUTC().toString("hh:mm:ss");
          ui->timer->setText(time);
      }

  }
//  qDebug()<< "updateTimer()>> " ;
}

void MainWindow::setInitSeconds(uint input ){
    if(input == -1){
        stopCountdownTimer = true;
    }else{
        stopCountdownTimer = false;
        initSeconds = input;
    }

}
void MainWindow::handleNewSocketConnection()
{
//! [5]
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

//! [4] //! [7]

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QIODevice::readyRead, this, &MainWindow::readSocket);



    clientConnection->write(block);

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

    qDebug()<< "Showing warningMesg() with: " << message;
    ui->statusBar->showMessage(message);
    QFont font;
    font.setBold(true);
    font.setPointSize(32);
//    QMessageBox::warning(nullptr, tr("Warning"), message);
    QMessageBox* msgBox = new QMessageBox( this );
       msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
       msgBox->setStandardButtons( QMessageBox::Ok );
       msgBox->setWindowTitle( tr("Warning") );
       msgBox->setText( message );
       msgBox->setFont(font);
       msgBox->setModal( false ); // if you want it non-modal
       msgBox->open( this, SLOT(msgBoxClosed(QAbstractButton*)) );

}


void MainWindow::msgBoxClosed(QAbstractButton *button) {
    qDebug()<< "msgBoxClosed() closed " ;
}






