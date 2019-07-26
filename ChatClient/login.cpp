#include "login.h"
#include "ui_login.h"
#include "client.h"
#include "mainwindow.h"
#include <bits/stdc++.h>
#include <string.h>
#include "okwindow.h"
#include <windows.h>
login::login(Client* ccl,QWidget *parent) :
    QDialog(parent),
    cl(ccl),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle(tr("Login Window"));
}

login::~login()
{
    delete ui;
}

void login::on_login_button_clicked()
{
    QString ID=ui->ID_edit->text(),pw=ui->password_edit->text(),fb;
    cl->SendMsgToServer("login");
    cl->GetFeedback(fb);
    if(fb=="0")
         ui->label_3->setText("Already logged,enjoy chatting!\n");
    else{
        if(ui->checkBox->isChecked()){
            cl->SendMsgToServer("Tourist");
            cl->GetFeedback(fb);
            if(ID=="")
                ID="游客"+QString::number(1);//静态变量出了些问题
            cl->SendMsgToServer(ID);
            cl->GetFeedback(fb);
            if(fb=="1"){
                cl->name=ID;
                okWindow ok("Quick Log in successfully\n");
                ok.exec();
                QDialog::accept();
            }else{
                ui->label_3->setText("This name is used");
            }
        }
        else{
            cl->SendMsgToServer(ID);
            cl->GetFeedback(fb);
            if(fb=="0")
                ui->label_3->setText("This user does not exist or is already online!\n");
            else{
                cl->SendMsgToServer(pw);
                cl->GetFeedback(fb);
                //okWindow ok(fb);ok.exec();
                if(fb=="0")
                    ui->label_4->setText("Password incorrect\n");
                else {
                    cl->name=ID;
                    okWindow ok("Log in successfully\n");
                    ok.exec();
                    QDialog::accept();
                }
            }
        }

    }
}

void login::on_Cancel_Button_clicked()
{
   QDialog::reject();
}

int login::on_register_button_clicked()
{
    rg=new reg(cl,this);
    return rg->exec();
}
