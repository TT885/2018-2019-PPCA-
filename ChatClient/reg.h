#ifndef REG_H
#define REG_H

#include <QDialog>
#include "client.h"
namespace Ui {
class reg;
}

class reg : public QDialog
{
    Q_OBJECT

public:
    explicit reg(Client *cll,QWidget *parent = nullptr);
    ~reg();

private slots:
    void on_RegOKButton_clicked();

private:
    Ui::reg *ui;
    Client* cl;
};

#endif // REG_H
