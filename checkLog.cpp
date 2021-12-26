#include "checkLog.h"

timerThread::timerThread(QObject *parent)
    : QThread(parent)
{
   lineCount = 0;
   justStarted = true;
}

timerThread::~timerThread()
{


    wait();
}



void timerThread::run()
{





}

void timerThread::readProcessOutput(){

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

void timerThread::processDead(){
    qDebug() << "process end!";
}



