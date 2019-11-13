#include "server.h"
#include "connectionthread.h"
#include "support.h"

#include <QNetworkInterface>
#include <QJsonDocument>
#include <QJsonObject>

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
    if( (privateServer && currentConnection < 2) || !privateServer)
    {
        QTcpSocket* socket = serverSocket->nextPendingConnection();

        ConnectionThread* ct = new ConnectionThread(socket, lastId++);

        currentConnection++;

        QJsonObject obj;
        obj["type"] = config::messageType::connection;
        obj["state"] = config::messageState::ok;
        obj["data"] = QString::number(lastId-1);
        if(currentConnection == 2 && privateServer)
            obj["initPS"] = config::messageState::startInit;
        else
            obj["initPS"] = config::messageState::none;

        socket->write(QJsonDocument(obj).toJson());

        connect(ct, &QThread::finished, ct, &QThread::deleteLater);

        connect(ct, &ConnectionThread::signalClientWrote, this, &Server::signalWriteToClients, Qt::QueuedConnection);
        connect(this, &Server::signalWriteToClients, ct, &ConnectionThread::slotSendToClient);

        connect(ct, &ConnectionThread::signalClientDisconnect, this, &Server::slotClientDisconnect);

        ct->start();
    }
    else if(privateServer && currentConnection == 2)
    {
        QTcpSocket* socket = serverSocket->nextPendingConnection();

        QJsonObject obj;
        obj["type"] = config::messageType::connection;
        obj["state"] = config::messageState::denied;
        obj["data"] = "No free slots on server. Disconnecting...";

        socket->write(QJsonDocument(obj).toJson());

        socket->close();
        socket->waitForBytesWritten();
        socket->deleteLater();
    }
}

void Server::slotClientDisconnect()
{
    currentConnection--;

    if(privateServer && currentConnection == 1);
    {
        QJsonObject obj;
        obj["type"] = config::messageType::passwordSharing;
        obj["state"] = config::messageState::close;

        emit signalWriteToClients(QJsonDocument(obj).toJson(), -1);
    }
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
