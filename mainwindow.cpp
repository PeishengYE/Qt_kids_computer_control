#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	manager = new QNetworkAccessManager(this);

    ftpAddress = "ftp://172.16.18.199/uploaded_from_phone/";
	ftpPort = 21;
    username = "yep";
    password = "4r5t6y";
    localFolder = "/home/yep/work/xmind_projects_all/securus_projects/log/Mini_UI_on_Security_patch/log/";
    downloadTest();


}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::downloadTest()
{
// Check folder
    downloadFileName ="yep.swimming.in.pool.mp4";

    if (localFolder != "" && QDir(localFolder).exists())
	{
		QUrl ftpPath;
		ftpPath.setUrl(ftpAddress + downloadFileName);
		ftpPath.setUserName(username);
		ftpPath.setPassword(password);
		ftpPath.setPort(ftpPort);

		QNetworkRequest request;
		request.setUrl(ftpPath);

		downloadFileReply = manager->get(request);
		connect(downloadFileReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadFileProgress(qint64,qint64)));
		connect(downloadFileReply, SIGNAL(finished()), this, SLOT(downloadFileFinished()));
	}
	else
	{
		QMessageBox::warning(this, "Invalid Path", "Please set the download path before download.");
	}

}


void MainWindow::downloadFileProgress(qint64 byteReceived,qint64 bytesTotal)
{
	qint64 percentage = 100 * byteReceived / bytesTotal;

    ui->patchProgress->setValue((int) percentage);
    ui->statusBar->showMessage(tr("Download with %1 percentage ").arg((int) percentage));
}

void MainWindow::downloadFileFinished()
{
	if(downloadFileReply->error() != QNetworkReply::NoError)
	{
		QMessageBox::warning(this, "Failed", "Failed to download file: " + downloadFileReply->errorString());
	}
	else
	{
		QByteArray responseData;
		if (downloadFileReply->isReadable())
		{
			responseData = downloadFileReply->readAll();
		}

		if (!responseData.isEmpty())
		{
			// Download finished

            QFile file(localFolder + "/" + downloadFileName);
			file.open(QIODevice::WriteOnly);
			file.write((responseData));
			file.close();

			QMessageBox::information(this, "Success", "File successfully downloaded.");
		}
	}
}
