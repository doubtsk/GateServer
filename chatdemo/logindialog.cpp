#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
loginDialog::loginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn,&QPushButton::clicked,this,&loginDialog::switchregister);
}

loginDialog::~loginDialog()
{
    qDebug()<<"destruct LoginDlg";
    delete ui;
}
