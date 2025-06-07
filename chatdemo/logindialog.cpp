#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
loginDialog::loginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn,&QPushButton::clicked,this,&loginDialog::switchregister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");
    connect(ui->forget_label, &ClickLabel::clicked, this, &loginDialog::slot_forget_pwd);
}

loginDialog::~loginDialog()
{
    qDebug()<<"destruct LoginDlg";
    delete ui;
}

void loginDialog::slot_forget_pwd()
{
    qDebug()<<"slot forget pwd";
    emit switchReset();
}
