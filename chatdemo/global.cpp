#include"global.h"
QString gate_url_prefix = "";
std::function<void(QWidget*)> repolish =[](QWidget* w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
};


std::function<QString(QString)>xorString=[](QString input)//简单加密
{
    QString result = input;
    int length =input.length();
    length = length % 255;
    for (int i = 0; i < length; ++i)
    {
        //每个字符都进行异或操作
        // 这里假设字符都是ASCII。因此直接转换成QChar
        result[i] = QChar(static_cast<ushort>(input[i].unicode()^static_cast<ushort>(length)));
    }
    return result;
};
