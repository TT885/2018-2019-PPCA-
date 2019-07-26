#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "okwindow.h"
#include "client.h"
#include "login.h"
#include "chooseip.h"
namespace Ui {
class MainWindow;
}
class login;
class MainWindow : public QMainWindow
{
    Q_OBJECT
friend class login;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionConnect_triggered();
    void on_actionLogin_triggered();
    void on_send_Button_pressed();
    void on_clear_Button_pressed();
    void newMsg(QString &); //Client收到信息转过来

    void on_info_text_textChanged();

    void on_img_Button_clicked();

    void on_VIPButton_clicked();

    void on_BanButton_2_clicked();

    void on_UnbanButton_3_clicked();

    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;
    Client *cl;
    login *lg;
    QDateTime current_Date_Time;
};

#endif // MAINWINDOW_H
