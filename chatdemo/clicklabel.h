#ifndef CLICKLABEL_H
#define CLICKLABEL_H
#include "global.h"
#include <QLabel>

class ClickLabel : public QLabel
{
    Q_OBJECT
public:
    ClickLabel(QWidget *parent =nullptr);
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    void SetState (QString nomral="",QString hover="",QString press="",
                  QString select="",QString select_hover="",QString select_press="");
    ClickLbState GetCurState();

private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _select_hover;
    QString _select_press;

    ClickLbState _curstate;
signals:
    void clicked(void);
};

#endif // CLICKLABEL_H
