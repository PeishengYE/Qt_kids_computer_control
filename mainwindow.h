#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>
#include <QtCore>
#include <QTcpServer>
#include <QTcpSocket>

#define MAGIC_NUM "889988"
#define POWER_OFF "POWER_OFF"
#define SHOW_MESG "SHOW_MESG"
#define SET_TIME_TO_POWER_OFF "SET_TIME_TO_POWER_DOWN"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
    void powerOffComputer();

    void showMessage(QString cmdstr);

private:
     void executeCmd(QString cmdStr);
     QString runCmd(QProcess *myProcess, QString program, QStringList& arguments);
    void setInitSeconds(uint input );

private slots:


    void warningMesg(QString errorMessage);
    void exitApp();
    void sendFortune();
    void readFortune();
    void updateTimer();


signals:
    void messageArrived(const QString &messg);

private:
	Ui::MainWindow *ui;
    QTimer *quitAppTimer;
    int m_timerId;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    uint initSeconds = 60*15;
    bool stopCountdownTimer = false;



};

#endif // MAINWINDOW_H
