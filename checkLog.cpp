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
    // Check folder
        QString logFilename = "/var/log/syslog";
        QFile file(logFilename);



        forever{

        if (!file.open(QFile::ReadOnly | QFile::Text)) {
          return;
          }

        QTextStream qin(&file);
        lineCount = 0;

        while(!qin.atEnd()) {
            QString line;
            line = qin.readLine();
            if (line.isNull())
                break;

            if (line.trimmed().isEmpty())
                continue;

            if (line.trimmed().contains("kernel", Qt::CaseInsensitive)){
                qDebug() << "find one kernel line: " << line.trimmed();
                lineCount ++;
                currentLine = line.trimmed();
                emit sendCounts(lineCount, currentLine);
                msleep(100);
            }

            if (lineCount > 50){
                emit done();
            }
        }
        file.close();

        }
    //		connect(downloadFileReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadFileProgress(qint64,qint64)));

    //		QMessageBox::warning(this, "Invalid Path", "Please set the download path before download.");


}

//            Block block(QRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1),
//                        QColor(red/n, green/n, blue/n));
//            emit sendBlock(block);
//            if (m_abort)
//                return;
//            msleep(10);


