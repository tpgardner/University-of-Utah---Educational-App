#include "loginwin.h"
#include "ui_loginwin.h"
#include "client.h"
#include <QMessageBox>

LoginWin::LoginWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWin) {
    ui->setupUi(this);
    ui->passEntry->setEchoMode(QLineEdit::Password);
    //this->setStyleSheet("background-color: black; color: white;");
}

LoginWin::LoginWin(Client *client, QWidget *parent) : LoginWin(parent) {
    this->client = client;
}

LoginWin::~LoginWin() {
    delete ui;
}

void LoginWin::on_loginButton_clicked() {
    Client *par = client;

    try{
    if (!par->sendLogin(ui->userEntry->text(), ui->passEntry->text())) {
        //TODO Make dialog
        QMessageBox messageBox;
        messageBox.critical(0,"ERROR", "Invalid Login Credentials");
        messageBox.setFixedSize(500,200);
        ui->passEntry->setText("");
    }else{

    }}catch(socketexception ex){
        QMessageBox messageBox;
        messageBox.critical(0,"ERROR", "Server not responding");
        messageBox.setFixedSize(500,200);
    }
}

void LoginWin::on_regStud_clicked() {
    Client *par = client;

    par->setCurrentPage("studreg");
}

void LoginWin::on_regTeach_clicked() {
    Client *par = client;

    par->setCurrentPage("teachreg");
}

void LoginWin::on_passEntry_returnPressed() {
    on_loginButton_clicked();
}
