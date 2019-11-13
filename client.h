#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>

#include "passwordsharing.h"
#include "support.h"

class Client : public QObject
{
    Q_OBJECT

private:
    unsigned id;
    QString nickname;
    QTcpSocket* socket;
    QHostAddress connectedAddress;

    PasswordSharing ps;
    bool cryptMessage;

    void parseDataFromServer(QByteArray byteMessage);
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
