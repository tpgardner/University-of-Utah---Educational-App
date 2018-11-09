#ifndef STUDREG_H
#define STUDREG_H

#include <QWidget>
#include <client.h>

namespace Ui {
class StudReg;
}

/**
 * @brief The StudReg class is a window for students to register a login name and password
 * with the Stackd game API.
 */
class StudReg : public QWidget
{
    Q_OBJECT

public:
    explicit StudReg(QWidget *parent = 0);
    StudReg(Client * client, QWidget *parent=0);
    ~StudReg();

private slots:
    void on_cancelButton_clicked();
    void on_regButton_clicked();

private:
    Ui::StudReg *ui;
     Client * client;
    bool Validate(QString s);
};

#endif // STUDREG_H
