#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>

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
	void downloadFileProgress(qint64 byteReceived,qint64 bytesTotal);
	void downloadFileFinished();
    void downloadTest();

private:
	Ui::MainWindow *ui;

	QNetworkAccessManager* manager;

	QString ftpAddress;
	int ftpPort;
	QString username;
	QString password;
    QString localFolder;

	QNetworkReply* downloadFileListReply;
	QNetworkReply* uploadFileListReply;

	QNetworkReply* uploadFileReply;
	QNetworkReply* downloadFileReply;

	QStringList fileList;
	QString uploadFileName;
	QString downloadFileName;
};

#endif // MAINWINDOW_H
