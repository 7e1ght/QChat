#include "server.h"
#include "connectionthread.h"
#include "config.h"

#include <QNetworkInterface>

bool Server::isPrivate()
{
    return privateServer;
}

QHostAddress Server::getIpAddress()
{
    return ipAddress;
}

void Server::slotNewConnection()
{
    QTcpSocket* socket = serverSocket->nextPendingConnection();
    ConnectionThread* ct = new ConnectionThread(socket, lastId++);

    connect(ct, &QThread::finished, ct, &QThread::deleteLater);

    connect(ct, &ConnectionThread::signalClientWrote, this, &Server::signalWriteToClients, Qt::QueuedConnection);
    connect(this, &Server::signalWriteToClients, ct, &ConnectionThread::slotSendToClient);

    connect(ct, &ConnectionThread::signalClientDisconnect, this, &Server::slotClientDisconnect);

    ct->start();

    currentConnection++;
}

void Server::slotClientDisconnect()
{
    currentConnection--;
}

//void Server::slotWriteToClients(QByteArray data, unsigned notToId)
//{
//    emit this->signalWriteToClients(data, notToId);
//}

bool Server::start()
{
    return serverSocket->listen(QHostAddress::Any, config::port);
}

Server::Server(bool privateServer)
{
    serverSocket = new QTcpServer;

    ipAddress = QNetworkInterface::allAddresses()[2];

    this->privateServer = privateServer;
    connect(serverSocket, &QTcpServer::newConnection, this, &Server::slotNewConnection);
}

Server::~Server()
{
    delete serverSocket;
}
