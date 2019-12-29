#include "floatpan.h"
#include "ui_floatpan.h"
/* 此类为悬浮窗函数 */
FloatPan::FloatPan(QWidget *parent,QString text) :
    QWidget(parent),
    ui(new Ui::FloatPan)
{
    ui->setupUi(this);
    ui->textEdit->setText(text);

    int id = QFontDatabase::addApplicationFont("/usr/local/qt5.4.1-enter/lib/fonts/DroidSansFallback.ttf");
    QString msyh = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug(msyh.toLatin1());
    QFont font(msyh,10);
    font.setPointSize(15);


    //QFont font;
    font.setPixelSize(20);
    font.setBold(true);
    ui->textEdit->setFont(font);
}

FloatPan::~FloatPan()
{
    delete ui;
}

void FloatPan::setCustomText(QString text)
{
    ui->textEdit->setText(text);
}


