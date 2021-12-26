#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDebug>

class ServerThread : public QThread
{
	Q_OBJECT
public:
    explicit ServerThread(QObject *parent = nullptr);
	void startServer();
	void sendMessageToClients(QString message);
     ~ServerThread();


signals:
    void receviedMesg(const QString &msg);
public slots:
	void newClientConnection();
	void socketDisconnected();
	void socketReadReady();
	void socketStateChanged(QAbstractSocket::SocketState state);
    QTcpServer* getInternalQTcpServer();

protected:
    void run();
private:
	QTcpServer* chatServer;
	QVector<QTcpSocket*>* allClients;

};

#endif // SERVER_H
