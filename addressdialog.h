#ifndef ADDRESSDIALOG_H
#define ADDRESSDIALOG_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class AddressDialog;
}

class AddressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressDialog(QWidget *parent = nullptr);
    ~AddressDialog();

private:
    Ui::AddressDialog *ui;

signals:
    void signalReturnAddress(QHostAddress address);
private slots:
    void on_buttonBox_accepted();
};

#endif // ADDRESSDIALOG_H
