#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydb.h"
#define DELAY 400000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //居中
    desktop = QApplication::desktop();
    /* 表示从数据库中得到的数据 */
    Id = 1;
    ui->setupUi(this);
    StartTestflag = 0;
    Powerflag = 0;
    ui->StartTest->setEnabled(false);//首先需要电源按钮按下
    ui->CalibrateBroadcast->setEnabled(false);//首先需要电源按钮按下

    /* 菜单栏连接槽函数 */
    connect(ui->OpenFile_menu,SIGNAL(triggered()),this,SLOT(OpenFile_fun()));//文件
    connect(ui->ShowFile_menu,SIGNAL(triggered()),this,SLOT(ShowFile_fun()));//文件
    connect(ui->SetTime_menu,SIGNAL(triggered()),this,SLOT(SetTime_fun()));//设置
    connect(ui->Register_menu,SIGNAL(triggered()),this,SLOT(Register_fun()));//设置
    connect(ui->BeginTest_menu,SIGNAL(triggered()),this,SLOT(BeginTest_fun()));//测试
    connect(ui->CalibrateBroadcast_menu,SIGNAL(triggered()),this,SLOT(CalibrateBroadcast_fun()));//测试
    connect(ui->ShowDb_menu,SIGNAL(triggered()),this,SLOT(ShowDb_menu_fun()));//测试

    /* 数据库界面 */
    y = new mydb();
    //主窗口中刷新按钮的信号 连接到数据库函数中 send_db_fun槽函数
    QObject::connect(ui->Refresh,SIGNAL(clicked(bool)),y,SLOT(send_db_fun()));//连接数据库的信号

    //数据库中发送 send_db信号 连接主窗口的槽函数 get_db槽函数
    QObject::connect(y,SIGNAL(send_db(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int)),this,SLOT(get_db(int,int,\
                    int,int,int,int,int,int,int,int,int,int,int,int,int,int)));//连接数据库的信号



    /******如果一个按钮连接了两个 不知道是按照 次数来的， 还是时间********/
    //QObject::connect(ui->Refresh,SIGNAL(clicked(bool)),y,SLOT(send_ttyO5_fun()));//连接数据库的信号
    //QObject::connect(y,SIGNAL(send_ttyO5(int)),this,SLOT(get_ttyO5(int)));//连接数据库的信号


    /* 主界面显示信息 */
    widget = new FloatPan();//悬浮窗
    widget->move((desktop->width() - widget->width())/2,(desktop->height() - widget->height())/2);

    /* 第一个label信号与槽函数的连接 */
    connect(ui->label,SIGNAL(show()),this,SLOT(showfloatwindow()));
    connect(ui->label,SIGNAL(hide()),this,SLOT(hidefloatwindow()));

    /* 第二个label信号与槽函数的连接 */
    connect(ui->label_2,SIGNAL(show()),this,SLOT(showfloatwindow()));
    connect(ui->label_2,SIGNAL(hide()),this,SLOT(hidefloatwindow()));



    /* 添加显示总表信息 */
    //在此需要建立一个框图 表示信息的控件


    /* 测试次数 */
    connect(ui->test_num_pushButton,SIGNAL(clicked(bool)),y,SLOT(send_num_fun()));
    connect(y,SIGNAL(send_num(int)),this,SLOT(get_num(int)));

}
void MainWindow::showfloatwindow()
{
    //widget->setCustomText(QString::fromLocal8Bit("测试\r\ntest\r\n测试"));
    //widget->setCustomText(QString::fromLocal8Bit("RS485\r\nNET\r\nRTC\r\nUSB\r\n"));

    /* 这里可以判断各项test的状态 */

    //这里会出现不能确定鼠标在哪个位置 可以根据是那个label
    widget->setCustomText(QString::fromLocal8Bit("OK\r\nOK\r\nOK\r\nOK\r\n"));
    widget->show();
}
void MainWindow::hidefloatwindow()
{
    widget->hide();
}


/* 绘制图形表示状态 */
    //从数据库传来的数据有route addr num
    //ttyO1 ttyO2 ttyO3 ttyO4 ttyO5
    //USB NET RTC
//void MainWindow::draw(int a,int b,int c,int d,int e,QString str)
void MainWindow::draw(int a,int b,int c,int d,int e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QPainter painter(this);

    QBrush brush(Qt::white);//初始为白色
    if(e == 1)//1红色 2绿色 3白色
    {
        brush.setColor(Qt::red);
    }
    else if(e == 2)
    {
        brush.setColor(Qt::green);
    }
    else if(e == 3)
    {
        brush.setColor(Qt::white);
    }


    /* 判断状态,显示颜色 */
    //可以根据tty05来判断颜色
       /*if(addr_in == 1)
    {

        brush.setColor(Qt::red);
    }
    else if(addr_in == 2)
    {
        brush.setColor(Qt::green);//之前到了这里
    }*/


    /*************************************/
    QPen pen(Qt::gray);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.setBrush(brush);
    //QPoint center(75,255);//第一个圆的中心点 X Y
    QPoint center(a,c);//第一个圆的中心点 X Y


    painter.drawEllipse(center,50,50);//绘制圆的大小


    //QRectF rect_text(55,240,40,30);////上面字X轴位置 Y轴位置
    QRectF rect_text(b,d,40,30);////上面字X轴位置 Y轴位置


    painter.drawRect(rect_text);
    painter.setFont(QFont("Arial",10));
    //pen.setColor(Qt::black);
    painter.setPen(pen);

    if(e == 1)//1红色 2绿色 3白色
    {
             painter.drawText(rect_text,Qt::AlignCenter,"FAIL");
    }
    else if(e == 2)
    {
             painter.drawText(rect_text,Qt::AlignCenter,"PASS");
    }
    else if(e == 3)
    {
             painter.drawText(rect_text,Qt::AlignCenter,"WAIT...");
    }

    //painter.drawText(rect_text,Qt::AlignCenter,str);
    /***************************************/
    /*
     *QPen pen2(Qt::gray);
    pen2.setWidth(5);
    painter.setPen(pen);
    painter.setBrush(brush2);
    QPoint center2(185,255);//第二个圆的中心点

    painter.drawEllipse(center2,50,50);
    QRectF rect_text2(165,240,40,30);////上面字X轴位置 Y轴位置
    painter.drawRect(rect_text2);
    painter.setFont(QFont("Arial",10));
    pen.setColor(Qt::black);
    painter.setPen(pen);*/
    /**************************************/

    /* 需要根据什么来判断 */
    /*
    if(addr_in == 1)    //判断需要外移 根据中继和地址
    {
        if(Id==10)
        {
             painter.drawText(rect_text,Qt::AlignCenter,"WAIT...");
        }
        else if(Id==1)
        {
            qDebug()<<"Id = "<<Id;
            //painter.drawText(rect_text,Qt::AlignCenter,"PASS");
            painter.drawText(rect_text,Qt::AlignCenter,"FAIL");
        }
        else if(Id==1)
        {
            qDebug()<<"Id = "<<Id;
            //painter.drawText(rect_text,Qt::AlignCenter,"PASS");
            painter.drawText(rect_text,Qt::AlignCenter,"FAIL");
        }
    }
    else if(Status == -1)
    {
        if(Id==3)
        {
            painter.drawText(rect_text,Qt::AlignCenter,"WAIT...");
        }
        else
        {
            painter.drawText(rect_text,Qt::AlignCenter,"FAIL");
        }
    }
    else if(Status == 0)
    {
        painter.drawText(rect_text,Qt::AlignCenter,"WAIT...");
    }
    */
}
/* 显示界面 */
void MainWindow::paintEvent(QPaintEvent *)
{//绘制每一个图像的需要不同的颜色
    //主界面中不需要根据中继和地址判断 以此类推
    //绘制之前就要确定什么颜色
    //定义颜色
    //1红色 FAIL
    //2绿色 pass
    //3白色 wait

    /* 需要根据什么来判断 */     //外移到这
    //更换判断条件 根据返回的结果
    draw(PONIT_X,QRectF_X,PONIT_Y,QRectF_Y,3);//1
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y,QRectF_Y,3);//2
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y,QRectF_Y,3);//3
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y,QRectF_Y,3);//4

    draw(PONIT_X,QRectF_X,PONIT_Y+110,QRectF_Y+110,3);//5
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+110,QRectF_Y+110,3);//6
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+110,QRectF_Y+110,3);//7
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+110,QRectF_Y+110,3);//8

    draw(PONIT_X,QRectF_X,PONIT_Y+220,QRectF_Y+220,3);//9
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+220,QRectF_Y+220,3);//10
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+220,QRectF_Y+220,3);//11
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+220,QRectF_Y+220,3);//12

    draw(PONIT_X,QRectF_X,PONIT_Y+330,QRectF_Y+330,3);//13
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+330,QRectF_Y+330,3);//14
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+330,QRectF_Y+330,3);//15
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+330,QRectF_Y+330,3);//16



    switch (result1_in) {
    case 8:
        draw(PONIT_X,QRectF_X,PONIT_Y,QRectF_Y,2);//1
        break;
    default:
            draw(PONIT_X,QRectF_X,PONIT_Y,QRectF_Y,1);//1
        break;
    }

    switch (result2_in) {
    case 8:
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y,QRectF_Y,2);//2
        break;
    default:
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y,QRectF_Y,1);//2
        break;
    }

    switch (result3_in) {
    case 8:
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y,QRectF_Y,2);//3
        break;
    default:
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y,QRectF_Y,1);//3
        break;
    }

    switch (result4_in) {
    case 8:
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y,QRectF_Y,2);//4
        break;
    default:
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y,QRectF_Y,1);//4
        break;
    }

    switch (result5_in) {
    case 8:
    draw(PONIT_X,QRectF_X,PONIT_Y+110,QRectF_Y+110,2);//5
        break;
    default:
    draw(PONIT_X,QRectF_X,PONIT_Y+110,QRectF_Y+110,1);//5
        break;
    }

    switch (result6_in) {
    case 8:
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+110,QRectF_Y+110,2);//6
        break;
    default:
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+110,QRectF_Y+110,1);//6
        break;
    }
    switch (result7_in) {
    case 8:
        draw(PONIT_X+220,QRectF_X+220,PONIT_Y+110,QRectF_Y+110,2);//7
        break;
    default:
        draw(PONIT_X+220,QRectF_X+220,PONIT_Y+110,QRectF_Y+110,1);//7
        break;
    }

    switch (result8_in) {
    case 8:

        draw(PONIT_X+330,QRectF_X+330,PONIT_Y+110,QRectF_Y+110,2);//8
        break;
    default:

        draw(PONIT_X+330,QRectF_X+330,PONIT_Y+110,QRectF_Y+110,1);//8
        break;
    }

    switch (result9_in) {
    case 8:
    draw(PONIT_X,QRectF_X,PONIT_Y+220,QRectF_Y+220,2);//9
        break;
    default:
    draw(PONIT_X,QRectF_X,PONIT_Y+220,QRectF_Y+220,1);//9
        break;
    }

    switch (result10_in) {
    case 8:
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+220,QRectF_Y+220,2);//10
        break;
    default:
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+220,QRectF_Y+220,1);//10
        break;
    }

    switch (result11_in) {
    case 8:
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+220,QRectF_Y+220,2);//11
        break;
    default:
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+220,QRectF_Y+220,1);//11
        break;
    }

    switch (result12_in) {
    case 8:
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+220,QRectF_Y+220,2);//12
        break;
    default:
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+220,QRectF_Y+220,1);//12
        break;
    }

    switch (result13_in) {
    case 8:
        draw(PONIT_X,QRectF_X,PONIT_Y+330,QRectF_Y+330,2);//13

        break;
    default:
        draw(PONIT_X,QRectF_X,PONIT_Y+330,QRectF_Y+330,1);//13

        break;
    }

    switch (result14_in) {
    case 8:

        draw(PONIT_X+110,QRectF_X+110,PONIT_Y+330,QRectF_Y+330,2);//14

        break;
    default:

        draw(PONIT_X+110,QRectF_X+110,PONIT_Y+330,QRectF_Y+330,1);//14

        break;
    }

    switch (result15_in) {
    case 8:

        draw(PONIT_X+220,QRectF_X+220,PONIT_Y+330,QRectF_Y+330,2);//15

        break;
    default:

        draw(PONIT_X+220,QRectF_X+220,PONIT_Y+330,QRectF_Y+330,1);//15

        break;
    }

    switch (result16_in) {
    case 8:

        draw(PONIT_X+330,QRectF_X+330,PONIT_Y+330,QRectF_Y+330,2);//16
        break;
    default:

        draw(PONIT_X+330,QRectF_X+330,PONIT_Y+330,QRectF_Y+330,1);//16
        break;
    }







/*
 *  draw(PONIT_X,QRectF_X,PONIT_Y,QRectF_Y,3);//1
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y,QRectF_Y,3);//2

    draw(PONIT_X+220,QRectF_X+220,PONIT_Y,QRectF_Y,3);//3
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y,QRectF_Y,3);//4

    draw(PONIT_X,QRectF_X,PONIT_Y+110,QRectF_Y+110,1);//5
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+110,QRectF_Y+110,1);//6
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+110,QRectF_Y+110,1);//7
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+110,QRectF_Y+110,1);//8

    draw(PONIT_X,QRectF_X,PONIT_Y+220,QRectF_Y+220,1);//9
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+220,QRectF_Y+220,1);//10
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+220,QRectF_Y+220,1);//11
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+220,QRectF_Y+220,1);//12

    draw(PONIT_X,QRectF_X,PONIT_Y+330,QRectF_Y+330,1);//13
    draw(PONIT_X+110,QRectF_X+110,PONIT_Y+330,QRectF_Y+330,1);//14
    draw(PONIT_X+220,QRectF_X+220,PONIT_Y+330,QRectF_Y+330,1);//15
    draw(PONIT_X+330,QRectF_X+330,PONIT_Y+330,QRectF_Y+330,3);//16
    */



}
/* 打开文件 */
void MainWindow::OpenFile_fun()
{
    qDebug()<<"fun_open";
}
/* 查看文件 */
void MainWindow::ShowFile_fun()
{
    qDebug()<<"fun_show";
}

/* 设置时间 */
void MainWindow::SetTime_fun()
{
    qDebug()<<"fun_time";
    S = new mysetting;
    S->setWindowTitle("配置界面");
    widget->move((desktop->width() - S->width())/2,(desktop->height() - S->height())/2);
    QObject::connect(S,SIGNAL(send_str_end(QString,QString)),this,SLOT(get_str_end(QString,QString)));
    S->show();

}
/* 注册测试 */
void MainWindow::Register_fun()
{
    qDebug()<<"fun_register";

}
/* 开始测试 */
void MainWindow::BeginTest_fun()
{
    qDebug()<<"fun_test";
}
/* 校准广播 */
void MainWindow::CalibrateBroadcast_fun()
{
    qDebug()<<"fun_broadcast";

}
/* 查看数据库 */
void MainWindow::ShowDb_menu_fun()
{
    qDebug()<<"fun_db";
    //y = new mydb("test");
    //y->show();
}

/* 配置按钮 */
void MainWindow::on_Setting_clicked()
{
    S = new mysetting;
    S->setWindowTitle("配置界面");

    widget->move((desktop->width() - S->width())/2,(desktop->height() - S->height())/2);
    QObject::connect(S,SIGNAL(send_str_end(QString,QString)),this,SLOT(get_str_end(QString,QString)));
    S->show();
}
/* 得到开机时间 关机时间 */
void MainWindow::get_str_end(QString str,QString end)
{
    str_1 = str;
    end_1 = end;
    ui->start_Label_2->setText(str_1);
    ui->end_Label_2->setText(end_1);
}

/* 注册按钮 */
void MainWindow::on_Register_clicked()
{

}
/* 查看按钮 */
void MainWindow::on_OpenDb_clicked()
{
    /*QDialog * newDialog=new QDialog(this);
    QSqlError err;
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/test.db");

    if(!db.open())
    {
        err = db.lastError();
        qDebug()<<err.text();
    }*/

    qDebug()<<"fun_db";
    //y = new mydb("test");
    //y->show();

    y = new mydb();
    y->show();
}

MainWindow::~MainWindow()
{
    free(buffer);
    delete ui;
}

/* 开始测试按钮 */
void MainWindow::on_StartTest_clicked()
{
    if(StartTestflag == 0)
    {/* 正在测试 */
        ui->StartTest->setText("停止测试");
        StartTestflag = 1;
        ui->Power->setEnabled(false);
        ui->CalibrateBroadcast->setEnabled(false);
        /* 启动进程 */

        progress = new myprogress;//启动485测试程序
    }
    else
    {
        ui->StartTest->setText("开始测试");
        StartTestflag = 0;
        ui->Power->setEnabled(true);
        ui->CalibrateBroadcast->setEnabled(true);
    }
}

/* 校准广播按钮 */
void MainWindow::on_CalibrateBroadcast_clicked()
{
    /* PID true  0x00a1;  PID2 false 0x00a2 | //#define PID 0x00a1//#define PID2 0x00a2 */
    //BYTE* rect = (BYTE*)malloc(1);
    /* if(m_instance->getTypePID())    m_instance->setWorkMode(CHECK_MOD);                //0xbb 工作模式 校验设置
     m_instance->OSD_open(OSD_MAIN_OPEN);                                               //0x10
     usleep(DELAY);
     usleep(DELAY);
     usleep(DELAY);
     usleep(DELAY);
     m_instance->OSD_open(OSD_AUTO_SET);                                                //0x18
     usleep(DELAY);
    //m_instance->OSD_setValue(OSD_BRIGHTNESS_SET, 120);
    //m_instance->OSD_readPosition(OSD_BRIGHTNESS_POSITION, 0, rect, 1);
    //usleep(DELAY);
    //m_instance->OSD_readPosition(OSD_BRIGHTNESS_POSITION, 0, rect, 1);
    //usleep(DELAY);
    //m_instance->OSD_close(OSD_BRIGHTNESS_OPEN);
    //usleep(DELAY);
    //m_instance->OSD_close(OSD_MAIN_OPEN);
    //usleep(DELAY);
     if(m_instance->getTypePID())    m_instance->setWorkMode(OUTPUT_MOD);               //0xaa 工作模式 输出设置

    //printf("brightness value: %d  %02x\n", (*rect), (*rect));
    //m_value = QString::number(*rect);
    //ui.label_value->setText(m_value);
    //free(rect);


 */
}

/* 电源按钮 */
void MainWindow::on_Power_clicked()
{
    /*buffer = (BYTE*)malloc(8);
    buffer[0] = 0x55;    buffer[1] = 0x01;    buffer[2] = 0x13;    buffer[3] = 0x00;
    buffer[4] = 0x00;    buffer[5] = 0x00;    buffer[6] = 0x00;    buffer[7] = 0x69;//关闭电源盒子指令*/
    p = new mypower;
    if(Powerflag == 0)
    {/* 正在供电 */
        ui->Power->setText("关闭电源");
        Powerflag = 1;
        ui->StartTest->setEnabled(true);
        ui->Setting->setEnabled(false);//设置按钮不可按下
        ui->Register->setEnabled(false);//注册按钮不可按下
        ui->CalibrateBroadcast->setEnabled(true);//校准广播不可按下


       // buffer[3] = buffer[4] = buffer[5] = buffer[6] = 0xff;    buffer[7] = 0x65;//开启电源盒子指令
    }
    else
    {
        ui->Power->setText("打开电源");
        Powerflag = 0;
        ui->StartTest->setEnabled(false);
        ui->Setting->setEnabled(true);//设置按钮可按下
        ui->Register->setEnabled(true);//注册按钮可按下
        ui->CalibrateBroadcast->setEnabled(false);//校准广播可按下

       // buffer[3] = buffer[4] = buffer[5] = buffer[6] = 0x00;    buffer[7] = 0x69;//关闭电源盒子指令
    }

}

/* 刷新 */
void MainWindow::on_Refresh_clicked()
{/* 主界面显示 */
    mydb *w;
    w = new mydb;
    //QObject::connect(w,SIGNAL(send_db(int,int)),this,SLOT(get_db(int,int)));
    //QObject::connect(w,SIGNAL(send_db(int,int)),this,SLOT(get_db(int,int)));
    qDebug()<<"ok";
    update();
}
void MainWindow::get_ttyO5(int data)
{

    qDebug()<<"data = "<<data;
}

void MainWindow::get_db(int result1,int result2,int result3,int result4,int result5,int result6,int result7,int result8,\
                        int result9,int result10,int result11,int result12,int result13,int result14,int result15,int result16)
{//得到数据库发来的信号 数据的槽函数
    result1_in = result1;
    result2_in = result2;
    result3_in = result3;
    result4_in = result4;
    result5_in = result5;
    result6_in = result6;
    result7_in = result7;
    result8_in = result8;
    result9_in = result9;
    result10_in = result10;
    result11_in = result11;
    result12_in = result12;
    result13_in = result13;
    result14_in = result14;
    result15_in = result15;
    result16_in = result16;

    qDebug()<<"result1_in = "<<result1;
    qDebug()<<"result2_in = "<<result2;
}
void MainWindow::get_num(int num)
{
    QString str = QString::number(num);

    ui->test_num_label_2->setText(str);


}

void MainWindow::on_test_num_pushButton_clicked()
{

}
