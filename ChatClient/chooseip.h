#ifndef CHOOSEIP_H
#define CHOOSEIP_H

#include <QDialog>
#include "client.h"
namespace Ui {
class chooseIP;
}

class chooseIP : public QDialog
{
    Q_OBJECT

public:
    explicit chooseIP(Client* cll,QWidget *parent = nullptr);
    ~chooseIP();

private slots:
    void on_pushButton_clicked();

private:
    Ui::chooseIP *ui;
    Client* cl;
};

#endif // CHOOSEIP_H
