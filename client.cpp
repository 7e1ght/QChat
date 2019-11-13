#include "client.h"

#include <QHostAddress>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>

void Client::parseDataFromServer(QJsonDocument data)
{
    using namespace config;

    QJsonObject obj = data.object();
    int type = obj["type"].toInt();
    QString message = obj["data"].toString();

    if(type == messageType::message)
    {
        QString resString;
        resString = "[" + QTime::currentTime().toString() + "] " + obj["nickname"].toString() + ": " + message;
        emit signalAddToScreen(resString);
    }
    else if(type == messageType::id)
        setId(message.toInt());
}

void Client::setId(unsigned id)
{
    this->id = id;
}

QString Client::getNickname()
{
    return nickname;
}

void Client::setNickname(QString nickname)
{
    this->nickname = nickname;
}

bool Client::connectToHost(QHostAddress ip)
{
    socket->connectToHost(ip, config::port);

    if(socket->waitForConnected(5000))
    {
        connectedAddress = ip;

        return true;
    } else
        return false;
}

QHostAddress Client::getConnectedAddress()
{
    return connectedAddress;
}

void Client::sendToServer(QString data, config::messageType type, config::messageState state)
{
    QJsonObject obj;
    obj["type"] = type;
    obj["data"] = data;

    switch(type)
    {
        case config::messageType::message:
        {
            obj["nickname"] = nickname;
            break;
        }
    }

    QJsonDocument doc(obj);

    socket->write(doc.toJson());
}

void Client::slotReadServer()
{
    QByteArray byteMessage = "";

    while(socket->bytesAvailable() > 0)
        byteMessage += socket->readAll();

    QJsonDocument message = QJsonDocument::fromJson(byteMessage);

    parseDataFromServer(message);
}

void Client::slotDisconnect()
{
    socket->close();

    emit signalAddToScreen("Server down.");
}

Client::Client()
{
    socket = new QTcpSocket;
    nickname = "Aноним";

    connect(socket, &QTcpSocket::readyRead, this, &Client::slotReadServer);
//    connect(socket, &QTcpSocket::disconnected, this, &Client::slotDisconnect);
}
