#ifndef GLOBAL_H
#define GLOBAL_H
/******************************************************************************
 *
 * @file       global.h
 * @brief      全局刷新函数
 *
 * @author     蘸醋猫猫
 * @date       2025/05/10
 * @history
 *****************************************************************************/

#include <QWidget>
#include <functional>
#include<QStyle>
#include <QRegularExpression>
#include <memory>
#include <iostream>
#include <mutex>
#include<QByteArray>
#include<QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QSettings>
extern std::function<void (QWidget*)> repolish;
extern std::function<QString(QString)> xorString;
enum ReqId
{
    ID_GET_VARIFY_CODE =1001,//获取验证码
    ID_REG_USER  =1002,//注册用户
    ID_RESET_PWD = 1003,
    ID_LOGIN_USER= 1004,
    ID_CHAT_LOGIN = 1005,
    ID_CHAT_LOGIN_RSP = 1006,
};

enum Modules
{
    REGISTERMOD = 0,
    RESETMOD = 1,
};
enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VARIFY_ERR = 5,
    TIP_USER_ERR = 6
};
enum ErrorCodes
{
    SUCCESS=0,
    ERR_JSON =1,//JSON解析失败
    ERR_NETWORK =2,//通用网络错误
};
enum ClickLbState
{
    Normal = 0,
    Selected = 1,
};

extern QString gate_url_prefix;

#endif // GLOBAL_H
