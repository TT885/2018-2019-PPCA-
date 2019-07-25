#include "chooseip.h"
#include "ui_chooseip.h"
#include "client.h"
#include "okwindow.h"
chooseIP::chooseIP(Client* cll,QWidget *parent) :
    QDialog(parent),
    cl(cll),
    ui(new Ui::chooseIP)
{
    ui->setupUi(this);
    setWindowTitle(tr("Connect to Server"));
}

chooseIP::~chooseIP()
{
    delete ui;
}

void chooseIP::on_pushButton_clicked()
{
    QString ip=ui->lineEdit->text();
    uint16_t port=ui->lineEdit_2->text().toUInt();
    cl->ConnectToServer(ip,port);
    okWindow ok("Connection successful!");
    ok.exec();
    close();
}
