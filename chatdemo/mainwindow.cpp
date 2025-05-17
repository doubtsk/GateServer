#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg =new loginDialog(this);
    setCentralWidget(_login_dlg);
    //_login_dlg->show();


    //创建和注册消息链接
    connect(_login_dlg,&loginDialog::switchregister,this,&MainWindow::SlotSwithchReg);
    _reg_dlg=new registerDialog(this);

    //设置成无边框
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _reg_dlg->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    // if(_login_dlg)
    // {
    //     delete _login_dlg;
    //     _login_dlg=nullptr;
    // }
    // if(_reg_dlg)
    // {
    //     delete _reg_dlg;
    //     _reg_dlg=nullptr;
    // }
}

void MainWindow::SlotSwithchReg()
{
    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}
