#include "reg.h"
#include "ui_reg.h"
#include "bits/stdc++.h"
#include "qfont.h"
#include "okwindow.h"
#include <windows.h>
reg::reg(Client* cll,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reg),
    cl(cll)
{
    ui->setupUi(this);
    setWindowTitle(tr("Register Window"));
}

reg::~reg()
{
    delete ui;
}

void reg::on_RegOKButton_clicked()
{
    QString ID=ui->lineEdit->text(),pw1=ui->lineEdit_2->text(),pw2=ui->lineEdit_3->text();
    ui->label_4->setText("");
    ui->label_5->setText("");
    ui->label_6->setText("");
    if(pw1.length()<4)
        ui->label_5->setText("Password too short\n");
    else if(pw2!=pw1)
        ui->label_6->setText("Password does not match\n");
    else {
        QString fb;
        cl->SendMsgToServer("reg");
        cl->GetFeedback(fb);
        if(fb=="0")
            ui->label_4->setText("Already logged,enjoy chatting!");
        else{
            cl->SendMsgToServer(ID);
            cl->GetFeedback(fb);
            //okWindow ok(fb);ok.exec();
            if(fb=="0")
                ui->label_4->setText("ID already used\n");
            else{
                cl->SendMsgToServer(pw1);
                okWindow ok("Registration is Successful,please log in\n");
                ok.exec();
                close();
            }
        }
    }
}
