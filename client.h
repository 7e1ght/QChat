#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include "config.h"

class Client : public QObject
{
    Q_OBJECT

private:
    unsigned id;
    QString nickname;
    QTcpSocket* socket;
    QHostAddress connectedAddress;

    void parseDataFromServer(QJsonDocument data);
    void setId(unsigned id);

public:
    QString getNickname();
    void setNickname(QString nickname);
    bool connectToHost(QHostAddress ip);
    QHostAddress getConnectedAddress();
    void sendToServer(QString data, config::messageType = config::messageType::message, config::messageState state = config::messageState::none);

    Client();

public slots:
    void slotReadServer();
    void slotDisconnect();

signals:
    void signalAddToScreen(QString data);
};

#endif // CLIENT_H
