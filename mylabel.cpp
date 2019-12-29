#include "mylabel.h"


mylabel::mylabel(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
}

void mylabel::enterEvent(QEvent * event)
{
    emit show();
}

void mylabel::leaveEvent(QEvent * event)
{
    emit hide();
}

//mylabel::~mylabel()
//{

//}


