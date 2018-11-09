#include "teachreg.h"
#include "ui_teachreg.h"
#include "client.h"
#include <QMessageBox>

TeachReg::TeachReg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeachReg) {
    ui->setupUi(this);
    //maybe get rid of this
    ui->passEntry->setEchoMode(QLineEdit::Password);

    //this->setStyleSheet("background-color: black; color: white;");
}

TeachReg::TeachReg(Client *client, QWidget *parent) : TeachReg(parent) {
    this->client = client;
}

TeachReg::~TeachReg() {
    delete ui;
}

void TeachReg::on_cancelButton_clicked() {
    Client *par = client;

    par->setCurrentPage("login");
}

void TeachReg::on_regButton_clicked() {
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
        QString data = ui->userEntry->text() + "," + ui->passEntry->text() + "," + ui->classEntry->text() + ",1," + ui->dobEntry->text();
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
            QString error = "Class Code: " + ui->classEntry->text() + " is already taken";
            QMessageBox messageBox;
            messageBox.critical(0, "ERROR", error);
            messageBox.setFixedSize(500, 200);
            ui->classEntry->setText("");
        }
    }
}
