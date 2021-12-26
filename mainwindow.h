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
#include "checkLog.h"
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


private slots:
    void patchingProgress(int count, QString line);
    void patchingFinished();
    void warningMesg(QString errorMessage);
    void exitApp();
    void sendFortune();


signals:
    void logOnPatchingProgress();
    void logOnPatchingFinished();
private:
	Ui::MainWindow *ui;
    QTimer *quitAppTimer;
    int m_timerId;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;

};

#endif // MAINWINDOW_H
