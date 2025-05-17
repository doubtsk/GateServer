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
enum ReqId
{
    ID_GET_VARIFY_CODE =1001,//获取验证码
    ID_REG_USER  =1002,//注册用户
};

enum Modules
{
    REGISTERMOD=0,
};

enum ErrorCodes
{
    SUCCESS=0,
    ERR_JSON =1,//JSON解析失败
    ERR_NETWORK =2,//通用网络错误
};

extern QString gate_url_prefix;

#endif // GLOBAL_H
