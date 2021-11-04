#ifndef CHECKLOG_H
#define CHECKLOG_H

#include <QThread>
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



protected:
    void run();

    int lineCount;
    QString currentLine;
};


#endif // CHECKLOG_H
