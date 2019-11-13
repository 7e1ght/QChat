#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "client.h"
#include "server.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Client* client;
    Server* server;

    void parseInput();

public slots:
    void slotWriteToUser(QString data);

private slots:
    void on_actionPublick_Server_triggered();
    void on_send_clicked();
    void on_actionConnect_triggered();
    void slotConnectToAddress(QHostAddress address);
    void on_actionPrivate_Server_triggered();
};

#endif // MAINWINDOW_H
