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
    , ui(new Ui::registerDialog),_countdown(5)
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
    ui->err_tip->clear();

    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->comfir_edit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->get_code_edit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");

    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");
    connect(ui->pass_visible, &ClickLabel::clicked, this, [this]() {
        auto state = ui->pass_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    connect(ui->confirm_visible, &ClickLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->GetCurState();
        if(state == ClickLbState::Normal){
            ui->comfir_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->comfir_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });
    // 创建定时器
    _countdown_timer = new QTimer(this);
    // 连接信号和槽
    connect(_countdown_timer, &QTimer::timeout, [this](){
        if(_countdown==0){
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回登录").arg(_countdown);
        ui->tip_lb->setText(str);
    });
}

registerDialog::~registerDialog()
{
    qDebug()<<"destruct RegisterDialog";
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
        qDebug()<< "user uid is  " << jsonObj["uid"].toString() ;
        qDebug()<< "email is " << email ;
        ChangeTipPage();
    });
}
void registerDialog::AddTipErr(TipErr te,QString tips)
{
    _tip_errs[te]=tips;
    showTip(tips,false);
}

void registerDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty())
    {
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(),false);
}

bool registerDialog::checkUserValid()
{
    if(ui->user_edit->text()=="")
    {
        AddTipErr(TipErr::TIP_USER_ERR,tr("用户名不能为空!"));
    }
    return true;
}

bool registerDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}


bool registerDialog::checkPassValid()
{
    auto pass = ui->pass_edit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool registerDialog::checkConfirmValid()
{
    auto pass =ui->pass_edit->text();
    auto confirm =ui->comfir_edit->text();

    if(confirm.length()<6||confirm.length()>15)
    {
        //提示长度不准确
        AddTipErr(TipErr::TIP_CONFIRM_ERR,tr("密码长度应为6-15"));
        return false;
    }
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match =regExp.match(confirm).hasMatch();
    if(!match)
    {
        AddTipErr(TipErr::TIP_CONFIRM_ERR,tr("不能包含非法字符"));
        return false;
    }
    DelTipErr(TipErr::TIP_CONFIRM_ERR);

    if(pass!=confirm)
    {
        AddTipErr(TipErr::TIP_PWD_ERR,tr("确认密码和密码不匹配"));
        return false;
    }
    else
    {
        DelTipErr(TipErr::TIP_PWD_ERR);
    }
    return true;
}
bool registerDialog::checkVarifyValid()
{
    auto pass = ui->get_code_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
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
    json_obj["user"]=ui->user_edit->text();//用户名
    json_obj["email"]=ui->email_edit->text();//邮箱
    json_obj["passwd"]=xorString(ui->pass_edit->text());//密码
    json_obj["confirm"]=xorString(ui->comfir_edit->text());//确认密码
    json_obj["varifycode"]=ui->get_code_edit->text();//验证码
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj,ReqId::ID_REG_USER,Modules::REGISTERMOD);
}

void registerDialog::ChangeTipPage()
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    //启动定时器，设置时间间隔为1000毫秒(1秒)
    _countdown_timer->start(1000);
}


void registerDialog::on_return_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}
void registerDialog::on_cancel_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}

