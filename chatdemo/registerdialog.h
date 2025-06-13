#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"
namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = nullptr);
    ~registerDialog();

private slots:
    void on_gain_btn_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_pushButton_3_clicked();

    void on_return_btn_clicked();
    void on_cancel_btn_clicked();

private:

    void ChangeTipPage();
    bool checkVarifyValid();
    bool checkUserValid();
    bool checkEmailValid();
    bool checkConfirmValid();
    bool checkPassValid();
    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr te);
    void initHttpHandlers();
    void showTip(QString str,bool b_ok);
    Ui::registerDialog *ui;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr,QString> _tip_errs;

    QTimer *_countdown_timer;
    int _countdown;

signals:
    void sigSwitchLogin();
};

#endif // REGISTERDIALOG_H
