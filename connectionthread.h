#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <QTcpSocket>
#include <QThread>

class ConnectionThread : public QThread
{
    Q_OBJECT
private:
    QTcpSocket* clientSocket;
    unsigned id;
public:
    void run();
    ConnectionThread(QTcpSocket* socket, unsigned id);
//    ~ConnectionThread();
public slots:
    void slotReadClient();
    void slotDisconnect();
    void slotSendToClient(QByteArray data, unsigned notToId);

signals:
    void signalClientWrote(QByteArray data, unsigned notToId);
    void signalClientDisconnect();
};

#endif // CONNECTIONTHREAD_H
