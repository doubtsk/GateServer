#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "resetdialog.h"
#include <QMainWindow>
#include <logindialog.h>
#include <registerdialog.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();
    void SlotSwitchLogin2();
private:
    Ui::MainWindow *ui;
    loginDialog * _login_dlg;
    registerDialog * _reg_dlg;
    ResetDialog * _reset_dlg;
};
#endif // MAINWINDOW_H
