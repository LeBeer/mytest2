#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QTimer>

#include "mydb.h"
#include "mysetting.h"
#include "mypower.h"


#include "osd_api.h"


#include "myregister.h"
#include "myprogress.h"

#include "mydisplay.h"


#include <QPainter>
#include <QPen>
#include <QPoint>

#include "floatpan.h"//悬浮窗
#include "mylabel.h"//新建自己的label类
#include <qdesktopwidget.h>//居中

#define PONIT_X 90
#define PONIT_Y 255
#define QRectF_X 70
#define QRectF_Y 240
namespace Ui {
class MainWindow;
}
//using namespace std;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /* 表示从数据库中得到的数据 */
    int result1_in;
    int result2_in;
    int result3_in;
    int result4_in;
    int result5_in;
    int result6_in;
    int result7_in;
    int result8_in;
    int result9_in;
    int result10_in;
    int result11_in;
    int result12_in;
    int result13_in;
    int result14_in;
    int result15_in;
    int result16_in;

    /* 画笔状态表示 */
    int Id;
    int Status;
    int ProState;
    int Procnt;

    explicit MainWindow(QWidget *parent = 0);


    void paintEvent(QPaintEvent *);

    //void draw(int a,int b,int c,int d);
    void draw(int a,int b,int c,int d,int e);

    FloatPan *widget;//悬浮窗

    /* 定义全局变量 */        //来表示数据库中传来的数据

    //居中
    QDesktopWidget *desktop;

    //






    ~MainWindow();
public slots:
    void on_Setting_clicked();

    void on_Register_clicked();

    void on_OpenDb_clicked();

    void on_StartTest_clicked();

    void on_CalibrateBroadcast_clicked();

    void on_Power_clicked();

    void get_str_end(QString str,QString end);

    void OpenFile_fun();
    void ShowFile_fun();
    void SetTime_fun();
    void Register_fun();
    void BeginTest_fun();
    void CalibrateBroadcast_fun();
    void ShowDb_menu_fun();

    void on_Refresh_clicked();
    void get_db(int result1,int result2,int result3,int result4,int result5,int result6,int result7,int result8,\
                int result9,int result10,int result11,int result12,int result13,int result14,int result15,int result16);
    void get_ttyO5(int ttyO5);
    void get_num(int num);



    void showfloatwindow();
    void hidefloatwindow();
private slots:


private slots:



    void on_test_num_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    int StartTestflag;
    int Powerflag;

    COSD_API           *m_instance;

    mydb *y;
    mysetting *S;
    mypower *p;

    BYTE*                   buffer;

    myregister *m;

    QString str_1;
    QString end_1;

    myprogress *progress;
    /* 绘图 */
    //QBrush brush;
    //QPainter painter;
    //QRectF rect_text;



    QTableView *dbView;
    mylabel *label;




};

#endif // MAINWINDOW_H
