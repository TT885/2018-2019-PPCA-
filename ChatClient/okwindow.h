#ifndef OKWINDOW_H
#define OKWINDOW_H

#include <QDialog>

namespace Ui {
class okWindow;
}

class okWindow : public QDialog
{
    Q_OBJECT

public:
    explicit okWindow(QString,QWidget *parent = nullptr);
    ~okWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::okWindow *ui;
};

#endif // OKWINDOW_H
