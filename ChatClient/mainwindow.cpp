#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cl=new Client(this); //construct client
    connect(cl,SIGNAL(NewMsgCome(QString&)),this,SLOT(newMsg(QString&)));
    ui->info_text->setText("TPT聊天室欢迎您！请点击连接按钮连接到服务器\n");
    setWindowIcon(QIcon(":/logo.ico"));
    setWindowTitle(tr("TBT Chatroom"));
    //ui->VIPBrowser->setText("VIP");
    //O(∩_∩)O哈哈~
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    chooseIP * cip=new chooseIP(cl,this);
    cip->exec();
}

void MainWindow::on_send_Button_pressed()
{
    cl->ChangeConnection(0);//断开自动收信，发消息时需要主动交互信息，这里有信息冲突的危险.
    cl->SendMsgToServer("chat");
    QString fb;
    cl->GetFeedback(fb);
    if(fb=="0")
        ui->info_text->append("Please log in first(Or you are banned to chat)\n");
    else {
        QString name=ui->comboBox->currentText();
        cl->SendMsgToServer(name);
        cl->GetFeedback(fb);
        if(fb=="0")
            ui->info_text->append("This user is not online\n");
        else{
            QString msg=ui->edit_text->toPlainText();
            if(msg=="")
                msg="你好，这是自动聊天";
            ui->edit_text->clear();
            cl->SendMsgToServer(msg);

        }
    }
    cl->ChangeConnection(1);
}

void MainWindow::on_img_Button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("请选择选择发送的图片文件"),"F:",tr("图片文件(*png *jpg);;""本本文件(*txt)"));

    QFile* localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
       okWindow ok("File open error!");
       ok.exec();
       return;
    }

    cl->ChangeConnection(0);//断开自动收信，发图片时需要主动交互信息
    cl->SendMsgToServer("img");
    QString fb;
    cl->GetFeedback(fb);
    if(fb=="0")
      ui->info_text->append("Please log in first(Or you are banned to chat)\n");
    else{
        QString name=ui->comboBox->currentText();
        cl->SendMsgToServer(name);
        cl->GetFeedback(fb);
        if(fb=="0")
            ui->info_text->append("This user is not online\n");
        else{
            QByteArray outBlock=localFile->readAll();
            cl->SendFileToServer(outBlock);
        }
    }
    cl->ChangeConnection(1);
}


void MainWindow::on_actionLogin_triggered()
{
    cl->ChangeConnection(0);//断开自动收信，登录时需要主动交互信息
    lg=new login(cl,this);
    if(lg->exec()){
        ui->info_text->append("登录成功！\n");
        setWindowTitle("TBT chatroom:"+cl->name);
    }else {
        ui->info_text->append("取消登录\n");
    }
    cl->ChangeConnection(1);
}


void MainWindow::on_clear_Button_pressed()
{
    ui->edit_text->clear();
}
void MainWindow::newMsg(QString &msg)
{
    cl->ChangeConnection(0);//断开自动收信，收到消息时需要主动交互信息
    QString name;
    if(msg=="chat"){
        cl->SendMsgToServer("receive");
        cl->GetFeedback(name);
        qDebug()<<"Name="<<name;
        cl->SendMsgToServer("1");
        QString info;
        cl->GetFeedback(info);
        qDebug()<<"Info="<<info;

        current_Date_Time = QDateTime::currentDateTime();
        ui->info_text->append(current_Date_Time.toString("yyyy-MM-dd hh:mm:ss")+"\n"+name+":"+info+"\n");
    }
    else if(msg=="img"){
        cl->SendMsgToServer("receive");
        cl->GetFeedback(name);
        cl->SendMsgToServer("1");
        QByteArray info;
        cl->GetFeedback(info);
        QFile *fp=new QFile("img.jpg");

        if(!fp->open(QFile::WriteOnly))
        {
           okWindow ok("Recerived file open error!");
           ok.exec();
        }
        else
        {
            fp->write(info);
            current_Date_Time = QDateTime::currentDateTime();
            ui->info_text->append(current_Date_Time.toString("yyyy-MM-dd hh:mm:ss")+"\nFile,Already saved\n");
            fp->close();
        }
    }
    cl->ChangeConnection(1);
}


void MainWindow::on_info_text_textChanged()
{
     ui->info_text->moveCursor(QTextCursor::End);
}



void MainWindow::on_VIPButton_clicked()
{
    cl->ChangeConnection(0);//断开自动收信，登录时需要主动交互信息
    cl->SendMsgToServer("VIP");
    QString fb;
    cl->GetFeedback(fb);
    if(fb=="1"){
        QString code=ui->edit_text->toPlainText();
        if(code=="")
            code="a";
        cl->SendMsgToServer(code);
        cl->GetFeedback(fb);
        if(fb=="1"){
             ui->info_text->append("You are now VIP! You can forbid others to chat");
             ui->VIPBrowser->setText("VIP");
             ui->VIPBrowser->setFont(QFont("宋体",20,QFont::Bold));
             ui->edit_text->clear();
        }
        else{
            ui->info_text->append("VIP code wrong!");
        }
    }
    else{
        ui->info_text->append("Please login first");
    }

    cl->ChangeConnection(1);
}

void MainWindow::on_BanButton_2_clicked()
{
    cl->ChangeConnection(0);//断开自动收信，登录时需要主动交互信息
    cl->SendMsgToServer("ban");
    QString fb;
    cl->GetFeedback(fb);
    if(fb=="1"){
        QString name=ui->edit_text->toPlainText();
        if(name=="")
            name="a";
        cl->SendMsgToServer(name);
        cl->GetFeedback(fb);
        if(fb=="1"){
             ui->edit_text->clear();
             ui->info_text->append("Successfully banned");
        }
        else{
            ui->info_text->append("User not online,need not to ban!");
        }
    }
    else{
        ui->info_text->append("You are not VIP,can not do this");
    }
    cl->ChangeConnection(1);
}

void MainWindow::on_UnbanButton_3_clicked()
{
    cl->ChangeConnection(0);//断开自动收信，登录时需要主动交互信息
    cl->SendMsgToServer("unban");
    QString fb;
    cl->GetFeedback(fb);
    if(fb=="1"){
        QString name=ui->edit_text->toPlainText();
        if(name=="")
            name="a";
        cl->SendMsgToServer(name);
        cl->GetFeedback(fb);
        if(fb=="1"){
             ui->info_text->append("Successfully lift the ban");
             ui->edit_text->clear();
        }
        else{
            ui->info_text->append("User not banned,need not to lift the ban!");
        }
    }
    else{
        ui->info_text->append("You are not VIP,can not do this");
    }
    cl->ChangeConnection(1);
}

void MainWindow::on_pushButton_pressed()
{
    ui->edit_text->setText("whose your daddy");
    MainWindow::on_VIPButton_clicked();
}
