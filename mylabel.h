#ifndef MYLABEL_H
#define MYLABEL_H

#include <QObject>
#include <QLabel>

class mylabel : public QLabel
{
    Q_OBJECT
public:
    explicit mylabel(QWidget *parent = 0);

    //~mylabel();

signals:
    void show();
    void hide();

public slots:

private:
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
};

#endif // MYLABEL_H
