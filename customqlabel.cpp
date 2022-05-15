#include "customqlabel.h"

void CustomQLabel::mousePressEvent(QMouseEvent* event)
{
    //qInfo("click");
    emit clicked();
}

void CustomQLabel::enterEvent(QEnterEvent *event)
{
    //qInfo("enter");
    emit mouseEnter();
}

void CustomQLabel::leaveEvent(QEvent *event)
{
    //qInfo("leave");
    emit mouseLeave();
}

