#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addressdialog.h"

#include <QTime>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("QChat");


    client = new Client;
    server = nullptr;

    connect(client, &Client::signalAddToScreen, this, &MainWindow::slotWriteToUser);
}

void MainWindow::slotWriteToUser(QString data)
{
    ui->inMessage->addItem(data);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseInput()
{
    QString out = ui->outMessage->text();

    if(out[0] == '/')
    {
        QStringList list = out.split(" ");
        if(out == "/clear")
            ui->inMessage->clear();

        else if(list[0] == "/set")
        {
            if(list[1] == "nickname")
                client->setNickname(list[2]);
        }

    }

    else
    {
        slotWriteToUser("[" +
                        QTime::currentTime().toString() +
                        "] " +
                        client->getNickname() +
                        ": " +
                        out);
        client->sendToServer(out);
    }
}

void MainWindow::on_send_clicked()
{
    parseInput();
    ui->outMessage->setText("");
}

void MainWindow::on_actionPublick_Server_triggered()
{
    if(QNetworkInterface::allAddresses().size() <= 2)
    {
        slotWriteToUser("Need internet connection.");
        return;
    }

    if(server)
    {
        client->slotDisconnect();
        delete server;
    }

    server = new Server;

    if(server->start())
    {
        slotWriteToUser("Public server successfully created.");
    }
    else
        slotWriteToUser("Failed to create public server.");

    slotConnectToAddress(server->getIpAddress());
}
void MainWindow::on_actionPrivate_Server_triggered()
{
    if(QNetworkInterface::allAddresses().size() <= 2)
    {
        slotWriteToUser("Need internet connection.");
        return;
    }

    if(server)
    {
        client->slotDisconnect();
        delete server;
    }

    server = new Server(true);

    if(server->start())
    {
        slotWriteToUser("Private server successfully created.");
    }
    else
        slotWriteToUser("Failed to create private server.");

    slotConnectToAddress(server->getIpAddress());
}

void MainWindow::slotConnectToAddress(QHostAddress address)
{
    if(client->getConnectedAddress() == address)
    {
        slotWriteToUser("Already connected to " + address.toString());
    } else {
        if(client->connectToHost(address))
            slotWriteToUser("Successfully connected to " + address.toString());
        else
            slotWriteToUser("Failed connection to " + address.toString());
    }
}

void MainWindow::on_actionConnect_triggered()
{
    AddressDialog* ad = new AddressDialog(this);
    connect(ad, &AddressDialog::signalReturnAddress, this, &MainWindow::slotConnectToAddress);
    ad->show();
}

