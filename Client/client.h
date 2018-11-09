#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <QMainWindow>
#include <usersocket.h>
#include <puzzlewindow.h>
#include <QStackedWidget>


namespace Ui {
class Client;
}

/**
 * @brief The Client class is the top-level, main window and client for the Stackd game API and server.
 */
class Client : public QMainWindow
{
    Q_OBJECT

public:
    QStackedWidget stackWidget;
    explicit Client(QWidget *parent = 0);
    void setCurrentPage(QString);
    bool sendLogin(QString user, QString pass);
    int sendReg(QString data);
    QVector<QString> getStudents(QString classcode);
    ~Client();
    std::string getSessionId();
    std::string username;

public slots:
    void autosave();

signals:
    void setWidget(QWidget *);


private:
    QTimer timer;
    std::string sessionid;

    Ui::Client *ui;
    std::atomic_bool check;
    //UserSocket currsock;
    std::vector<QWidget*> widgets;
    unsigned int activeWidget = 0;
private slots:


    void loginWindow();
    void teacherWindow();
    void studentWindow();
    void studentRegistration();
    void teacherRegistration();
    void on_pushButton_7_clicked();
};

#endif // CLIENT_H
