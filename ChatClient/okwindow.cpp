#include "okwindow.h"
#include "ui_okwindow.h"

okWindow::okWindow(QString msg,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::okWindow)
{
    ui->setupUi(this);
    ui->label->setText(msg);
    setWindowTitle(tr("OK"));
}

okWindow::~okWindow()
{
    delete ui;
}

void okWindow::on_pushButton_clicked()
{
    close();
}
