#ifndef MYPOWER_H
#define MYPOWER_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

typedef unsigned char        BYTE;

class mypower : public QMainWindow
{
    Q_OBJECT
public:
    //explicit mypower(BYTE *buffer,QWidget *parent = 0);
    //explicit mypower(QString str,QWidget *parent = 0);
    explicit mypower(QWidget *parent = 0);
    ~mypower();
    void sendInfo(const QString &info);
    //QByteArray HEXStringToArr(QString hexstr);
    void StringToHex(QString str,QByteArray &senddata);
    char ConvertHexChar(char ch);

signals:
public slots:

private:
    QSerialPort *m_serialPort;
    BYTE* buffer_in;
        QByteArray byte;
};

#endif // MYPOWER_H
