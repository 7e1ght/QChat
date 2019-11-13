#include "client.h"

#include <QHostAddress>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>

#include "crypt.h"

void Client::parseDataFromServer(QByteArray byteMessage)
{
    using namespace config;

    qDebug() << "Client: " + byteMessage;

    QJsonDocument data = QJsonDocument::fromJson(byteMessage);

    qDebug() << data;

    QJsonObject obj = data.object();
    int type = obj["type"].toInt();
    int state = obj["state"].toInt();

    QString message = obj["data"].toString();


    if(cryptMessage)
    {
        message = Crypt::decrypt(message, ps.getK());
    }


    if(type == messageType::message)
    {
        QString resString;
        resString =
                "[" +
                QTime::currentTime().toString() +
                "] " +
                obj["nickname"].toString() +
                " " +
                obj["id"].toString() +
                ": " +
                message;
        emit signalAddToScreen(resString);
    }
    else if(type == messageType::id)
    {
        setId(message.toInt());
    }
    else if (type == messageType::connection)
    {
        if(state == messageState::ok)
        {
            id = message.toInt();

            if(obj["initPS"] == messageState::startInit)
            {
                sendToServer(QString::number(ps.getG()) +
                             " " + QString::number(ps.getP()) +
                             " " + QString::number(ps.calcOpenKey()),
                             messageType::passwordSharing,
                             messageState::init);
                qDebug() << "start init";
            } else {
                cryptMessage = false;
            }

        }
        else if(state == messageState::denied)
        {
            socket->close();
            emit signalAddToScreen(message);
        }
        else
        {
            emit signalAddToScreen("Server connection state error.  ConnectinState = " +
                                   QString::number(state));
        }
    }
    else if (type == messageType::passwordSharing)
    {

        if(state == messageState::init)
        {
            QStringList param = message.split(" ");
            ps = PasswordSharing(param[0].toInt(), param[1].toInt(), param[2].toInt());
            sendToServer(QString::number(ps.calcOpenKey()),
                         messageType::passwordSharing,
                         messageState::sendingOpenKey);

            cryptMessage = true;
            emit signalAddToScreen("Режим шифрования включен");
        }
        else if (state == messageState::sendingOpenKey)
        {
            ps.setK(message.toInt());

            cryptMessage = true;
            emit signalAddToScreen("Режим шифрования включен");
        }
        else if(state == messageState::close)
        {
            cryptMessage = false;
        }
    }
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

    return socket->waitForConnected(5000);
}

QHostAddress Client::getConnectedAddress()
{
    return connectedAddress;
}

void Client::sendToServer(QString data, config::messageType type, config::messageState state)
{
    QJsonObject obj;
    obj["type"] = type;
    obj["state"] = state;

    if(cryptMessage)
        obj["data"] = Crypt::encrypt(data, ps.getK());
    else
        obj["data"] = data;

    switch(type)
    {
        case config::messageType::message:
        {
            obj["nickname"] = nickname;
            obj["id"] = QString::number(id);
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

    parseDataFromServer(byteMessage);
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

    cryptMessage = false;

    connect(socket, &QTcpSocket::readyRead, this, &Client::slotReadServer);
    connect(socket, &QTcpSocket::disconnected, this, &Client::slotDisconnect);
}
