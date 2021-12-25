#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>
#include "checkLog.h"

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

signals:
    void logOnPatchingProgress();
    void logOnPatchingFinished();
private:
	Ui::MainWindow *ui;
    QTimer *quitAppTimer;
    int m_timerId;

};

#endif // MAINWINDOW_H
