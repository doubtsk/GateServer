#include "clicklabel.h"
#include <QMouseEvent>
ClickLabel::ClickLabel(QWidget *parent):QLabel(parent),_curstate(ClickLbState::Normal)
{

}

void ClickLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(_curstate==ClickLbState::Normal)
        {
            qDebug()<<"clicked,change to selected hover:"<<_select_hover;
            _curstate = ClickLbState::Selected;
            setProperty("state",_select_hover);
            repolish(this);
            update();
        }
        else
        {
            qDebug()<<"clicked,change to normal hover: "<<_normal_hover;
            _curstate=ClickLbState::Normal;
            setProperty("state",_normal_hover);
            repolish(this);
            update();
        }
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}

void ClickLabel::enterEvent(QEnterEvent *event)
{
    if(_curstate == ClickLbState::Normal)
    {
        qDebug()<<"enter,change to normal hover: "<<_normal_hover;
        setProperty("state",_normal_hover);
        repolish(this);
        update();
    }
    else
    {
        qDebug()<<"enter,change to selected hover: "<<_select_hover;
        setProperty("state",_select_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

void ClickLabel::leaveEvent(QEvent *event)
{
    if(_curstate == ClickLbState::Normal)
    {
        qDebug()<<"leave,change to normal"<<_normal;
        setProperty("state",_normal);
        repolish(this);
        update();
    }
    else
    {
        qDebug()<<"leave,change to normal hover"<<_normal_hover;
        setProperty("state",_normal_hover);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal=normal;
    _normal_hover=hover;
    _normal_press=press;

    _selected = select;
    _select_hover = select_hover;
    _select_press = select_press;

    setProperty("state",normal);
    repolish(this);
}

ClickLbState ClickLabel::GetCurState()
{
    return _curstate;
}
