#include "mypower.h"
#include <QDebug>
#include <QTextCodec>
#include <QByteArray>
/* 控制电源串口 */
mypower::mypower(QWidget *parent) : QMainWindow(parent)
{
    QStringList m_serialPortName;//这样就获取到可用的串口名字了
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        qDebug()<<"serialPortName:"<<info.portName();
    }
    m_serialPort = new QSerialPort();

    //if(m_serialPort->isOpen())
    //{
    //  m_serialPort->clear();
    // m_serialPort->close();
    // }

    /* 选择串口 */
    //A335x上控制电源的是ttyO1 也就是第1路RS485
    m_serialPort->setPortName(m_serialPortName[1]);//设置串口名字 假设我们上面已经成功获取到了 并且使用第一个
    qDebug()<<" m_serialPort->setPortName(m_serialPortName[1])"<< m_serialPortName[1];

    //PC端显示ttyS0 A335x上控制电源的是ttyO1 也就是第1路RS485
    //m_serialPort->setPortName(m_serialPortName[0]);
    //qDebug()<<" m_serialPort->setPortName(m_serialPortName[0])"<< m_serialPortName[0];

    if(m_serialPort->open(QIODevice::ReadWrite))
    {
        m_serialPort->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
        m_serialPort->setDataBits(QSerialPort::Data8);		//数据位为8位
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
        m_serialPort->setParity(QSerialPort::NoParity);	//无校验位
        m_serialPort->setStopBits(QSerialPort::OneStop); //一位停止位
        //qDebug()<<m_serialPortName[1]<<"Entered open ok!";
        //return;//串口在第一次打开失败 return 之后才会执行下面的
    }
    /************************************/
    buffer_in = (BYTE*)malloc(8);
    //QByteArray byte;
    //byte.resize(8);
    //for(int j=0;j++;j<8)
    //{
        //byte[j] = buffer_in[j];
    //}

    QByteArray byArr;

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QString text = codec->toUnicode(ba.constData(), ba.size(), &state);


    //QTextCodec *codec = QTextCodec::codecForName("System"); //获取系统编码
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec);

    QString dataStr = "\x55\x01\x13\x00\x00\x00\x00\x69";
    QString dataStr2("550113ffffffff65");
    //QString dataStr2 = "550113ffffffff65";
    byte.resize(8);
    StringToHex(dataStr2,byte);
    /************************************/
    qDebug()<<"byte = "<< byte.toHex();

    //byArr = dataStr2.toLatin1();
    byArr = dataStr2.toUtf8();
    /* error */
    m_serialPort->write(byte);//向串口写
    m_serialPort->close();//关闭串口
}
void mypower::StringToHex(QString str, QByteArray &senddata) //字符串转换为十六进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len / 2);
    char lstr,hstr;
    for (int i = 0; i < len; ) {
        hstr = str[i].toLatin1();
        if (hstr == ' ') {
            ++i;
            continue;
        }
        ++i;
        if (i  >= len) break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if ((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16 + lowhexdata;
        ++i;
        senddata[hexdatalen] = (char)hexdata;
        ++hexdatalen;
    }
    senddata.resize(hexdatalen);
}
char mypower::ConvertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - 0x30;
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch - 'A' + 10;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    else return ch -  ch;
}
mypower::~mypower()
{
    free(buffer_in);
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }
    delete m_serialPort;

}

