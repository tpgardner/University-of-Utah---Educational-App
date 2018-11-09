#ifndef LOGINWIN_H
#define LOGINWIN_H

#include <QWidget>
#include <client.h>

namespace Ui {
class LoginWin;
}

/**
 *@brief LoginWin class is the login window for the Stackd game API. It is the entry point for students
 * and teachers to access the register window as well as login to the game.
 */
class LoginWin : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWin(QWidget *parent = 0);
    LoginWin(Client * client, QWidget *parent=0);
    ~LoginWin();

private slots:
    void on_loginButton_clicked();

    void on_regStud_clicked();

    void on_regTeach_clicked();

    void on_passEntry_returnPressed();

private:
    Ui::LoginWin *ui;
    Client * client;
};

#endif // LOGINWIN_H
