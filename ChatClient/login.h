#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "client.h"
#include "mainwindow.h"
#include "reg.h"
namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT
public:
    explicit login(Client* ccl,QWidget *parent = nullptr);
    ~login();

private slots:
    void on_login_button_clicked();

    void on_Cancel_Button_clicked();

    int on_register_button_clicked();

private:
    Ui::login *ui;
    Client *cl;
    reg *rg;
};
#endif // LOGIN_H
