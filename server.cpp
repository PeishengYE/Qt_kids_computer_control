#include "server.h"

ServerThread::ServerThread(QObject *parent)
         : QThread(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketState>();
}

ServerThread::~ServerThread()
{



}

void ServerThread::startServer()
{
	allClients = new QVector<QTcpSocket*>;

    chatServer = new QTcpServer();
	chatServer->setMaxPendingConnections(10);
	connect(chatServer, SIGNAL(newConnection()), this, SLOT(newClientConnection()));

    if (chatServer->listen(QHostAddress::Any, 8005))
	{
        qDebug() << "Server has started. Listening to port 8005.";
	}
	else
	{
		qDebug() << "Server failed to start. Error: " + chatServer->errorString();
	}

    chatServer->waitForNewConnection();
}

QTcpServer* ServerThread::getInternalQTcpServer(){
    return chatServer;
}
void ServerThread::run()
{
    startServer();
    exec();
}

void ServerThread::sendMessageToClients(QString message)
{
	if (allClients->size() > 0)
	{
		for (int i = 0; i < allClients->size(); i++)
		{
			if (allClients->at(i)->isOpen() && allClients->at(i)->isWritable())
			{
				allClients->at(i)->write(message.toUtf8());
			}
		}
	}
}

void ServerThread::newClientConnection()
{
    qDebug() << "Socket new connection!";
	QTcpSocket* client = chatServer->nextPendingConnection();
	QString ipAddress = client->peerAddress().toString();
	int port = client->peerPort();

	connect(client, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(client, SIGNAL(readyRead()),this, SLOT(socketReadReady()));
	connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

	allClients->push_back(client);

	qDebug() << "Socket connected from " + ipAddress + ":" + QString::number(port);
}

void ServerThread::socketDisconnected()
{
	QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
	QString socketIpAddress = client->peerAddress().toString();
	int port = client->peerPort();

	qDebug() << "Socket disconnected from " + socketIpAddress + ":" + QString::number(port);
}

void ServerThread::socketReadReady()
{
	QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
	QString socketIpAddress = client->peerAddress().toString();
	int port = client->peerPort();

	QString data = QString(client->readAll());

	qDebug() << "Message: " + data + " (" + socketIpAddress + ":" + QString::number(port) + ")";
    emit receviedMesg(data);
    //sendMessageToClients(data);
}

void ServerThread::socketStateChanged(QAbstractSocket::SocketState state)
{
	QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
	QString socketIpAddress = client->peerAddress().toString();
	int port = client->peerPort();

	QString desc;

	if (state == QAbstractSocket::UnconnectedState)
		desc = "The socket is not connected.";
	else if (state == QAbstractSocket::HostLookupState)
		desc = "The socket is performing a host name lookup.";
	else if (state == QAbstractSocket::ConnectingState)
		desc = "The socket has started establishing a connection.";
	else if (state == QAbstractSocket::ConnectedState)
		desc = "A connection is established.";
	else if (state == QAbstractSocket::BoundState)
		desc = "The socket is bound to an address and port.";
	else if (state == QAbstractSocket::ClosingState)
		desc = "The socket is about to close (data may still be waiting to be written).";
	else if (state == QAbstractSocket::ListeningState)
		desc = "For internal use only.";


	qDebug() << "Socket state changed (" + socketIpAddress + ":" + QString::number(port) + "): " + desc;
}
