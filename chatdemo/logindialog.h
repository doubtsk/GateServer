#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "global.h"
#include <QDialog>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();

private:
    void initHttpHandlers();
    void initHead();
    Ui::loginDialog *ui;
    bool checkUserValid();
    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr te);
    bool checkPwdValid();
    bool enableBtn(bool enabled);
    QMap<TipErr,QString> _tip_errs;
    QMap<ReqId,std::function<void (const QJsonObject&)>>_handlers;
    int _uid;
    QString _token;
public slots:
    void slot_forget_pwd();
    void showTip(QString str,bool b_ok);
signals:
    void switchregister();
    void switchReset();
    void sig_connect_tcp(ServerInfo);
private slots:
    void on_sign_btn_clicked();
    void slot_login_mod_finish(ReqId id,QString res,ErrorCodes err);
    void slot_tcp_con_finish(bool success);
    void sig_login_failed(int);
};

#endif // LOGINDIALOG_H
