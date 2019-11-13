#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT
private:
    QTcpServer* serverSocket;
    unsigned currentConnection = 0;
    QHostAddress ipAddress;
    bool privateServer;
    unsigned lastId = 0;

public:
    QHostAddress getIpAddress();
    bool isPrivate();
    Server(bool privateServer = false);
    ~Server();

public slots:
    void slotNewConnection();
    void slotClientDisconnect();
//    void slotWriteToClients(QByteArray data, unsigned notToId);
    bool start();

signals:
    void signalWriteToClients(QByteArray data, unsigned notToId);
};

#endif // SERVER_H
