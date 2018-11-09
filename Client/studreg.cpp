#include "studreg.h"
#include "loginwin.h"
#include "client.h"
#include "ui_studreg.h"
#include "QGridLayout"
#include "QPushButton"
#include <QMessageBox>
#include <QRegExp>

StudReg::StudReg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudReg) {
    ui->setupUi(this);
    //maybe get rid of this
    ui->passEntry->setEchoMode(QLineEdit::Password);
    //this->setStyleSheet("background-color: black; color: white;");
}

StudReg::StudReg(Client *client, QWidget *parent) : StudReg(parent) {
    this->client = client;
}

StudReg::~StudReg() {
    delete ui;
}

void StudReg::on_cancelButton_clicked() {
    Client *par = client;

    par->setCurrentPage("login");
}

void StudReg::on_regButton_clicked() {
    QRegExp re("[A-Za-z0-9]{4,16}");

    if (!re.exactMatch(ui->userEntry->text())) {
        QMessageBox messageBox;
        messageBox.critical(0, "ERROR", "Usernames must be 4+ alphanumeric characters");
        messageBox.setFixedSize(500, 200);
        ui->userEntry->setText("");
    } else if (!re.exactMatch(ui->passEntry->text())) {
        QMessageBox messageBox;
        messageBox.critical(0, "ERROR", "Passwords must be 4+ alphanumeric characters");
        messageBox.setFixedSize(500, 200);
        ui->passEntry->setText("");
    } else {
        Client *par = client;
        QString data = ui->userEntry->text() + "," + ui->passEntry->text() + "," + ui->classEntry->text() + ",0," + ui->dobEntry->text();
        int x = par->sendReg(data);

        //username taken
        if (x == 1) {
            QString error = "Username: " + ui->userEntry->text() + " is already taken";
            QMessageBox messageBox;
            messageBox.critical(0, "ERROR", error);
            messageBox.setFixedSize(500, 200);
            ui->userEntry->setText("");
        }
        //class code taken
        else if (x == 2) {
            QString error = "Class Code: " + ui->classEntry->text() + " is invalid";
            QMessageBox messageBox;
            messageBox.critical(0, "ERROR", error);
            messageBox.setFixedSize(500, 200);
            ui->classEntry->setText("");
        }
    }
}

//alphanumeric 4-16
bool StudReg::Validate(QString) {
    return true;
}
