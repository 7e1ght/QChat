#include "connectionthread.h"
#include "support.h"

#include <QThread>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>

void ConnectionThread::slotReadClient()
{
    QByteArray message = "";

    while(clientSocket->bytesAvailable() > 0)
            message += clientSocket->readAll();

    qDebug() << "Server: " + message;

    emit signalClientWrote(message, id);
}

void ConnectionThread::slotSendToClient(QByteArray data, unsigned notToId)
{
    if(id != notToId)
        clientSocket->write(data);

    clientSocket->waitForBytesWritten(5000);
}

void ConnectionThread::run()
{
    connect(clientSocket, &QTcpSocket::readyRead, this, &ConnectionThread::slotReadClient, Qt::DirectConnection);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ConnectionThread::slotDisconnect, Qt::DirectConnection);

    exec();
}

void ConnectionThread::slotDisconnect()
{
    clientSocket->close();
//    clientSocket->deleteLater(); //

    emit signalClientDisconnect();

    quit();
}

ConnectionThread::ConnectionThread(QTcpSocket* socket, unsigned id) :
    clientSocket(socket), id(id)
{


}

//ConnectionThread::~ConnectionThread()
//{
//    qDebug() << "Уничтожения потока с клиентом";
//    delete clientSocket;
//}
