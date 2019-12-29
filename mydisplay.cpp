#include "mydisplay.h"
#include <QPalette>
#include <QPainter>
#include <QMouseEvent>
mydisplay::mydisplay(QWidget *parent) : QWidget(parent)
{
    ui->setupUi(this);
    //setWindowFilePath(Qt::FramelessWindowHint);

    myRed = Qt::red;
    myGreen = Qt::red;
    myBlue = Qt::red;
    myIndex = 0;

    myPoints.clear();
    myDrawing = false;

}

mydisplay::~mydisplay()
{

}
void mydisplay::drawLine()
{
    myType = true;
    QColor  color("white");
    QPalette  palette;
    palette.setColor(QPalette::Window, color);
    setPalette(palette);
}

void mydisplay::paintEvent(QPaintEvent * event)
{
    if( myType && myDrawing )
    {
        QPainter painter(this);
        painter.setPen("black");
        //painter.setPen(Qt::SolidLine);
        //painter.drawLines(m_points);
        for(int i = 0; i < myPoints.size()-1; i++)
        {
            painter.drawLine(myPoints[i], myPoints[i+1]);
        }
    }
}

void mydisplay::displayRGB()
{
    myIndex = 0;
    myType = false;
    changeColorSlot();
    //m_timer = new QTimer();
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(changeColorSlot()));
    //m_timer->start(1000);
}

void mydisplay::changeColorSlot()
{
    QColor  color;
    if(0 == myIndex)
    {
        color = myRed;
    }
    else if(1 == myIndex)
    {
        color = myGreen;
    }
    else if(2 == myIndex)
    {
        color = myBlue;
    }
    else
    {
        close();
    }

    QPalette  palette;
    palette.setColor(QPalette::Window, color);
    setPalette(palette);
}

void mydisplay::mousePressEvent(QMouseEvent * event)
{
    if( !myType )
    {
        myIndex++;
        changeColorSlot();
    }
    else
    {
        myDrawing = true;
        myPoints.push_back(event->pos());
    }
    //update();
    repaint();
}

void mydisplay::mouseMoveEvent(QMouseEvent * event)
{
    if( myType && myDrawing )
    {
        myPoints.push_back(event->pos());
    }
    //update();
    repaint();
}

void mydisplay::mouseReleaseEvent(QMouseEvent * event)
{
    if( myType && myDrawing )
    {
        myDrawing = false;
        myPoints.clear();
    }
    update();
}

void mydisplay::mouseDoubleClickEvent(QMouseEvent * event)
{
    if( myType )
    {
        close();
    }
}

