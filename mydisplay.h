#ifndef MYDISPLAY_H
#define MYDISPLAY_H

#include <QWidget>
#include <QTimer>
#include <QVector>
//#include "ui_mydisplay.h"

namespace Ui {
class mydisplay;
}

class mydisplay : public QWidget
{
    Q_OBJECT

public:
    explicit mydisplay(QWidget *parent = 0);
    ~mydisplay();
    void drawLine();
    void displayRGB();
public slots:
    void changeColorSlot();

private:
    Ui::mydisplay *ui;
    QTimer *mytimer;
    QColor myRed;
    QColor myGreen;
    QColor myBlue;
    int myIndex;

    bool myType;
    bool myDrawing;
    QVector<QPoint> myPoints;

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent * event);


};

#endif // MYDISPLAY_H
