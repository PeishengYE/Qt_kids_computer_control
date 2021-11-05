#include "checkLog.h"

CheckLogThread::CheckLogThread(QObject *parent)
    : QThread(parent)
{
   lineCount = 0;
   justStarted = true;
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
    if(justStarted){
        qDebug().noquote() << "Just start, ignore all reading! ";
        justStarted = false;
        return;
    }

    if (line.contains("autoupdate", Qt::CaseInsensitive)){




                    /* Read the message from golang */
                   if(line.contains("1998886", Qt::CaseInsensitive)){
                       qDebug() << "Patching is done " << line;
                       emit warningMesg("Patching has finished successfully!");

                   }

                   if(line.contains("1998889", Qt::CaseInsensitive)){
                       qDebug() << "Patching with error, stopped!" << line;
                       emit warningMesg("Patching with error, stopped!");

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



