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
private:
    void initHttpHandlers();
    void showTip(QString str,bool b_ok);
    Ui::registerDialog *ui;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;

};

#endif // REGISTERDIALOG_H
