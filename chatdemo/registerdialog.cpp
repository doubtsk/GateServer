#include"global.h"
#include "registerdialog.h"
#include "ui_registerdialog.h"
#include"httpmgr.h"
/******************************************************************************
 *
 * @file       registerdialog.cpp
 * @brief      注册界面
 *
 * @author     蘸醋猫猫
 * @date       2025/05/10
 * @history
 *****************************************************************************/
registerDialog::registerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerDialog)
{
    ui->setupUi(this);
    //使注册界面的密码两行不可见
    ui->pass_edit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->comfir_edit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reg_mod_finish,
            this,&registerDialog::slot_reg_mod_finish);
    initHttpHandlers();
}

registerDialog::~registerDialog()
{
    delete ui;
}

void registerDialog::on_gain_btn_clicked()
{
    auto email=ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match =regex.match(email).hasMatch();
    if (match)
    {
        //发送验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),
                                            json_obj, ReqId::ID_GET_VARIFY_CODE,Modules::REGISTERMOD);
    }
    else
    {
        showTip(tr("邮箱地址不正确!"),false);
    }
}

void registerDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络请求错误"),false);
        return;
    }

    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if(jsonDoc.isNull()){
        showTip(tr("json解析错误"),false);
        return;
    }

    //json解析错误
    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        return;
    }

    _handlers[id](jsonDoc.object());

    //调用对应的逻辑
    return;
}

void registerDialog::initHttpHandlers()
{
    //注册获取验证码回包逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](QJsonObject jsonObj){
        qDebug() << "Received JSON:" << jsonObj;
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"), true);
        qDebug()<< "email is " << email ;
    });
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug()<< "email is " << email ;
    });
}

void registerDialog::showTip(QString str,bool b_ok)
{
    if (b_ok)
    {
        ui->err_tip->setProperty("state","normal");
    }
    else
    {
        ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}


void registerDialog::on_pushButton_3_clicked()
{
    if(ui->user_edit->text()=="")
    {
        showTip(tr("用户名不能为空"),false);
        return;
    }
    if(ui->email_edit->text()=="")
    {
        showTip(tr("邮箱不能为空"),false);
        return;
    }
    if(ui->pass_edit->text()=="")
    {
        showTip(tr("密码不能为空"),false);
        return;
    }
    if(ui->comfir_edit->text()=="")
    {
        showTip(tr("确认密码不能为空"),false);
        return;
    }
    if(ui->pass_edit->text()!=ui->comfir_edit->text())
    {
        showTip(tr("密码和确认密码不匹配"),false);
        return;
    }

    QJsonObject json_obj;
    json_obj["user"]=ui->user_edit->text();
    json_obj["email"]=ui->email_edit->text();
    json_obj["passwd"]=ui->pass_edit->text();
    json_obj["confirm"]=ui->comfir_edit->text();
    json_obj["varifycode"]=ui->get_code_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj,ReqId::ID_REG_USER,Modules::REGISTERMOD);
}

