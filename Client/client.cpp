#include "client.h"
#include "loginwin.h"
#include "studwin.h"
#include "teachwin.h"
#include "studreg.h"
#include "teachreg.h"
#include "ui_client.h"
#include "QGraphicsView"
#include "QGridLayout"
#include "QPushButton"
#include <usersocket.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include <QDesktopWidget>
#include <QDebug>
#include <QUrl>
#include <QDesktopServices>
#include <QTimer>

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client) {
    ui->setupUi(this);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &Client::loginWindow);
    QObject::connect(ui->pushButton_2, &QPushButton::clicked, this, &Client::studentWindow);
    QObject::connect(ui->pushButton_3, &QPushButton::clicked, this, &Client::teacherWindow);
    QObject::connect(ui->pushButton_3, &QPushButton::clicked, this, &Client::studentRegistration);
    QObject::connect(ui->pushButton_4, &QPushButton::clicked, this, &Client::teacherRegistration);
    QObject::connect(ui->pushButton_7, &QPushButton::clicked, this, &Client::on_pushButton_7_clicked);
    QObject::connect(this,&Client::setWidget,&stackWidget, &QStackedWidget::setCurrentWidget);
    QObject::connect(&timer, &QTimer::timeout, this, &Client::autosave);
    //move the window to the center of the screen
    move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

    LoginWin * lwin = new LoginWin(this,nullptr);
    StudWin * swin = new StudWin(this,nullptr);
    TeachWin* twin = new TeachWin(this,nullptr);
    StudReg * sreg = new StudReg(this,nullptr);
    TeachReg * treg = new TeachReg(this,nullptr);

    widgets.push_back(lwin);
    widgets.push_back(swin);
    widgets.push_back(twin);
    widgets.push_back(sreg);
    widgets.push_back(treg);
    stackWidget.addWidget(lwin);
    stackWidget.addWidget(swin);
    stackWidget.addWidget(twin);
    stackWidget.addWidget(sreg);
    stackWidget.addWidget(treg);
    timer.start(30000); //every 3 seconds or so
    this->setStyleSheet("background-color: black; color: white");
    this->setCentralWidget(&stackWidget);
    setCurrentPage("login");
    check = true;
}

Client::~Client() {
    delete ui;
    try {
        UserSocket sock(sf::IpAddress::LocalHost, 11777, getSessionId());
        sock.deauthenticate();
    } catch (std::exception) {
    }
}

//TODO: Move this all to controller later?
void Client::setCurrentPage(QString s) {
    if (check.is_lock_free() && check) {
        check = false;
        if (s == "login") {
            activeWidget = 0;
        } else if (s == "studwin") {
            activeWidget = 1;
            StudWin* win = static_cast<StudWin*>(stackWidget.widget(activeWidget));
            this->stackWidget.removeWidget(win);
            win = new StudWin(this,nullptr);
            stackWidget.addWidget(win);
            widgets[activeWidget]=win;
            win->switched();
        } else if (s == "teachwin") {
            activeWidget = 2;
            static_cast<TeachWin *>(stackWidget.widget(activeWidget))->updateStudents();
        } else if (s == "studreg") {
            activeWidget = 3;
        } else if (s == "teachreg") {
            activeWidget = 4;
        }
        emit setWidget(widgets[activeWidget]);
        stackWidget.currentWidget()->setFocus();
        stackWidget.currentWidget()->activateWindow();
        check = true;
    }
}

void Client::autosave() {
    if (activeWidget == 1 && sessionid != "") {
        UserSocket sock(sf::IpAddress::LocalHost, 11777, sessionid);
        StudWin *win = static_cast<StudWin *>(widgets[1]);
        std::vector<bool> solvedlist = win->getSolvedList();
        int index = 0;
        for (auto it = solvedlist.begin(); it < solvedlist.end(); it++) {
            bool i = *it;
            if (i) {
                std::stringstream ss;
                ss << username << "," << index+1;
                Message msg = sock.sendPayload("puzzlesolved", ss.str());
            }
            index++;
        }
        win->updatePuzzles();
    }
}

bool Client::sendLogin(QString user, QString pass) {
    bool teach = false;
    try {
        UserSocket sock(sf::IpAddress::LocalHost, 11777);
        sock.authenticate(user.toStdString(), pass.toStdString()); //if no exceptions thrown, then we are authenticated
        sessionid = sock.sid();
        username = user.toStdString();
        std::string payload = user.toStdString()+ "," + pass.toStdString();
        std::string command = "teachAuthenticate";
        Message msg = sock.sendPayload(command, payload);
        if(msg.payload=="VALID"){
            teach = true;
        }
        // sock.deauthenticate(); //when you are done deauthenticate, or save the sid for later
        //(note: the server will be configured to auto check for expired session ids -- probably every like 20 minutes or something )
    } catch (authenticationexception) { // if the client was not authenticated properly, or the session key was invalid
        return false;
    }
    //send payload and parse payload to determine if teach/student

    if (teach) {
        setCurrentPage("teachwin");
        static_cast<TeachWin *>(stackWidget.currentWidget())->setCurrentUsername(QString::fromStdString(username));
    } else {
        setCurrentPage("studwin");
        StudWin * stud = static_cast<StudWin *>(stackWidget.currentWidget());
        stud->updatePuzzles();
        static_cast<StudWin *>(stackWidget.currentWidget())->setCurrentUsername(QString::fromStdString(username));
    }

    return true;
}

int Client::sendReg(QString data) {
    UserSocket sock(sf::IpAddress::LocalHost, 11777);
    try {
        Message msg = sock.sendPayload("register", data.toStdString());
    } catch (reguserexception) {
        return 1;
    } catch (regclassexception) {
        return 2;
    } catch(socketexception){
        QMessageBox messageBox;
        messageBox.critical(0,"ERROR", "Server Not Reponding");
        messageBox.setFixedSize(500,200);
        return 3;
    }
    setCurrentPage("login");
    return 0;
}

//hardcoded to class "" for now
QVector<QString> Client::getStudents(QString) { // still unimplemented
    UserSocket sock(sf::IpAddress::LocalHost, 11777,getSessionId());
    Message msg = sock.sendPayload("getstudents", "");//classcode.toStdString());
    QVector<QString> students = QString::fromStdString(msg.payload).split(",").toVector();
    return students;
}

//UI debug individual pages
void Client::loginWindow() {
    setCurrentPage("login");
}

void Client::studentWindow() {
    setCurrentPage("studwin");
}

std::string Client::getSessionId() {
    return sessionid;
}

void Client::teacherWindow() {
    setCurrentPage("teachwin");
}

void Client::studentRegistration() {
    setCurrentPage("studreg");
}

void Client::teacherRegistration() {
    setCurrentPage("teachreg");
}

void Client::on_pushButton_7_clicked() {
    UserSocket sock(sf::IpAddress::LocalHost, 11777);

    try {
        sock.authenticate("averysecretusername", "averysecretusername");
        qDebug() << "tried to send payload";
        Message msg = sock.sendPayload("studentlist", "");
        QDesktopServices::openUrl(QUrl(QString::fromStdString(msg.payload)));
    } catch (...) {
        QMessageBox messageBox;
        messageBox.critical(0,"ERROR", "Server Not Responding");
        messageBox.setFixedSize(500,200);
    }
}
