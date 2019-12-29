#include "mysetting.h"


mysetting::mysetting(QWidget *parent) : QMainWindow(parent)
{
    timebox = createTimeCroup();
    numberbox = creatNumberGroup();
    layout = new QGridLayout;
    layout->addWidget(timebox, 0, 0, 2, 1 );
    layout->addWidget(numberbox, 0,1, 2, 1);
    widget = new QWidget;
    widget->setLayout(layout);

    //widget->setWindowTitle("配置界面");
    setCentralWidget(widget);
    resize(400,300);



}

QGroupBox* mysetting::createTimeCroup()
{
    QGroupBox *box = new QGroupBox(tr("设置时间"));
    QGridLayout *layout = new QGridLayout;
    QLabel *label_1 = new QLabel(tr("开机时间"));
    QLabel *label_2 = new QLabel(tr("关机时间"));

    downbox = new QComboBox;
    downbox->addItem("1");
    downbox->addItem("2");
    downbox->addItem("3");
    downbox->addItem("4");
    downbox->addItem("5");
    downbox->addItem("6");
    downbox->addItem("7");
    downbox->addItem("8");
    downbox->addItem("9");
    downbox->addItem("10");
    downbox_1 = new QComboBox;
    downbox_1->addItem("1");
    downbox_1->addItem("2");
    downbox_1->addItem("3");
    downbox_1->addItem("4");
    downbox_1->addItem("5");

    layout->addWidget(downbox, 0, 1, 2, 1 );
    layout->addWidget(downbox_1,1,1,2,1);
    layout->addWidget(label_1,0,0,2,1);
    layout->addWidget(label_2,1,0,2,1);
    box->setLayout(layout);
    return box;
}

QGroupBox* mysetting::creatNumberGroup()
{
    QGroupBox *box = new QGroupBox(/*tr("设置次数")*/);

    QPushButton *quit = new QPushButton(tr("退出"));
    QPushButton *enter = new QPushButton(tr("确定"));

    connect(quit,SIGNAL(clicked(bool)),this,SLOT(quit_func()));
    connect(enter,SIGNAL(clicked(bool)),this,SLOT(enter_func()));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(quit, 0, 1, 2, 1 );
    layout->addWidget(enter, 0, 0, 2, 1 );
    box->setLayout(layout);
    return box;

}
void mysetting::quit_func()
{
    //free(newDialog);
    this->close();

}
void mysetting::enter_func()
{
    QString str = downbox->currentText();
    qDebug()<< str;
    QString end = downbox_1->currentText();
    qDebug()<< end;
    emit send_str_end(str,end);
    this->close();
}

mysetting::~mysetting()
{

}

