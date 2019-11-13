#include "addressdialog.h"
#include "ui_addressdialog.h"

#include <QRegExp>
#include <QMessageBox>

AddressDialog::AddressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressDialog)
{
    ui->setupUi(this);

    setWindowTitle("Address");
}

AddressDialog::~AddressDialog()
{
    delete ui;
}

void AddressDialog::on_buttonBox_accepted()
{
    QString address = ui->lineEdit->text();
    QRegExp exp("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
    if(address.contains(exp))
        emit signalReturnAddress(QHostAddress(address));
    else
        QMessageBox::critical(nullptr, "Неверный адрес", "Вы ввели неверный адрес сервера");
}
