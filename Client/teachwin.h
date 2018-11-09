

#ifndef TEACHWIN_H
#define TEACHWIN_H

#include <QWidget>
#include <QDesktopWidget>
#include <QDebug>
#include <QUrl>
#include <QDesktopServices>
#include <client.h>
#include <QMessageBox>


namespace Ui {
class TeachWin;
}

/**
 * @brief The StudReg class is a window for teachers to login and gather analytics on students
 * registered in the Stackd game API.
 */
class TeachWin : public QWidget
{
    Q_OBJECT

public:
    void deleteStudent(std::string);
    void updateStudents();
    TeachWin(Client * client, QWidget *parent=0);
    explicit TeachWin(QWidget *parent = 0);
    ~TeachWin();
    void setCurrentUsername(QString currentUsername);

private slots:
    void logoutButton();
    void addStudentButton();
    void on_listWidget_itemSelectionChanged();
    void deleteStudentButton();

    void on_addStudentButton_clicked();

    void on_analyticsButton_clicked();

private:
    Ui::TeachWin *ui;
    Client * client;
    QString currentUsername;
};

#endif // TEACHWIN_H
