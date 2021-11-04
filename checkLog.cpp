#include "checkLog.h"

CheckLogThread::CheckLogThread(QObject *parent)
    : QThread(parent)
{
   lineCount = 0;
}

CheckLogThread::~CheckLogThread()
{


    wait();
}



void CheckLogThread::run()
{

        QString program = "tail";
        QStringList arguments;
        lineCount = 0;
         arguments << "-f" << "/var/log/syslog" ;
//          arguments << "-h"  ;
         myProcess = new QProcess(nullptr);

         myProcess->start(program, arguments);

         qDebug().noquote() <<"Thread started!";

         connect(myProcess, &QProcess::readyReadStandardOutput, this, &CheckLogThread::readProcessOutput);


         qDebug() << "waiting for finishing... " << endl;
         if (!myProcess->waitForFinished(-1)){
             qDebug() << "error on waitForFinished()." << endl;
         }

         qDebug() << "done !... " << endl;





}

void CheckLogThread::readProcessOutput(){

    qDebug().noquote() <<"readProcessOutput()>> ";
    QByteArray lines = myProcess->readAll();
    QString line = QString(lines);
    qDebug().noquote() << "[*]: " << QString(lines);
    if (line.contains("autoupdate", Qt::CaseInsensitive)){


                   if(line.contains("Critical error", Qt::CaseInsensitive)){
                       qDebug() << "AutoUpdate with critical error: " << line;
                       emit warningMesg(line);
                       msleep(5000);
                       emit done();
                   }


                   qDebug() << "AutoUpdate:" << line;
                   lineCount ++;
                   currentLine = line;
                   emit sendCounts(lineCount, currentLine);
                   msleep(100);
               }


}

void CheckLogThread::processDead(){
    qDebug() << "process end!";
}



