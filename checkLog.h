#ifndef CHECKLOG_H
#define CHECKLOG_H

#include <QThread>
#include <QtCore/QProcess>
#include <QFile>
#include <QDebug>
#include <QtCore/QTextStream>
class CheckLogThread : public QThread
{
    Q_OBJECT

public:
    CheckLogThread(QObject *parent = 0);
    ~CheckLogThread();



signals:
    void sendCounts(const int & count, const QString & line);
    void done();
    void warningMesg(const QString &err);

public slots:
    void readProcessOutput();
    void processDead();

protected:
    void run();

    int lineCount;
    QProcess *myProcess;
    QString currentLine;
};


#endif // CHECKLOG_H
