#include "mydraw.h"
#include <QStyle>

mydraw::mydraw(): QMainWindow(parent)
{
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        QPainter painter(this);
        QBrush brush(Qt::white);//初始为白色

        /* 判断状态 */
        if(Status == -1)
        {

                brush.setColor(Qt::red);

        }
        else if(Status == -1)
        {

             brush.setColor(Qt::green);//之前到了这里

        }
        QPen pen(Qt::gray);
        pen.setWidth(6);
        painter.setPen(pen);
        painter.setBrush(brush);
        QPoint center(200,364);
        painter.drawEllipse(center,120,120);
        QRectF rect_text(80+40,244+90,160,60);////上面字X轴位置 Y轴位置
        painter.drawRect(rect_text);
        painter.setFont(QFont("Arial",30));
        pen.setColor(Qt::black);
        painter.setPen(pen);
        if(Status == 1)
        {
            if(Id==3)
            {
                 painter.drawText(rect_text,Qt::AlignCenter,"WAIT...");

            }
            else
            {
                 painter.drawText(rect_text,Qt::AlignCenter,"PASS");

            }
        }
        else if(Status == -1)
        {

            if(Id==3)
            {

                //painter.drawText(rect_text,Qt::AlignCenter,"WAIT...");

            }
            else
            {

                painter.drawText(rect_text,Qt::AlignCenter,"FAIL");
                ProState = 0;

                if((Id==5)||(Id<5))
                {
                    Procnt =1;

                    ProState = 0;
                    if(Id<5)
                    {

                    }
                }
            }
        }
        else if(Status == 0)
        {
            painter.drawText(rect_text,Qt::AlignCenter,"WAIT...");
        }

}

mydraw::~mydraw()
{

}

