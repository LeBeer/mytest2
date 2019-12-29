#include "mydb.h"

mydb::mydb(QDialog *parent) : QMainWindow(parent)
//mydb::mydb(const QString &testTable,QDialog *parent) : QMainWindow(parent)
{
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
    }
    qDebug()<<"数据库窗口";

    testModel = new QSqlTableModel(this);
    testModel->setTable("test");//数据库表
    testModel->select();
    setWindowFlags(Qt::FramelessWindowHint);
    QGroupBox *test = createTestCroup();
    QGroupBox *detail = creatDetailGroup();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(test, 0, 0, 2, 1 );
    layout->addWidget(detail, 0,1, 2, 1);
    layout->setColumnMinimumWidth(0, 630);//设置数据采集和详细信息的大小 原始大小500
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    resize(1024,768);
    setHeader();//设置表头

    //需要在数据库类中组合好视图
    dataMapper = new QDataWidgetMapper();
    dataMapper->setModel(testModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    QLabel *label;
    label = new QLabel;
    //dataMapper->addMapping(label,testModel->fieldIndex("route"));
    qDebug()<<"label = "<<label->text().toInt();
    /* ERROR */

}
void mydb::setHeader()
{/* 设置表头 */
    testModel->setHeaderData(testModel->fieldIndex("route"),Qt::Horizontal,"中继");
    testModel->setHeaderData(testModel->fieldIndex("addr"),Qt::Horizontal,"地址");
    testModel->setHeaderData(testModel->fieldIndex("num"),Qt::Horizontal,"测试次数");
    testModel->setHeaderData(testModel->fieldIndex("ttyO1"),Qt::Horizontal,"485_1");
    testModel->setHeaderData(testModel->fieldIndex("ttyO2"),Qt::Horizontal,"485_2");
    testModel->setHeaderData(testModel->fieldIndex("ttyO3"),Qt::Horizontal,"485_3");
    testModel->setHeaderData(testModel->fieldIndex("ttyO4"),Qt::Horizontal,"485_4");
    testModel->setHeaderData(testModel->fieldIndex("ttyO5"),Qt::Horizontal,"485_5");
    testModel->setHeaderData(testModel->fieldIndex("CreatedTime"),Qt::Horizontal,"测试时间");
}
QGroupBox* mydb::createTestCroup()
{
    testView = new QTableView;
    testView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    testView->setSortingEnabled(true);
    testView->setSelectionBehavior(QAbstractItemView::SelectRows);
    testView->setSelectionMode(QAbstractItemView::SingleSelection);
    testView->setShowGrid(true);
    testView->setAlternatingRowColors(true);
    testView->setModel(testModel);
    connect(testView,SIGNAL(clicked(QModelIndex)),this,SLOT(changeTestView(QModelIndex)));

    QGroupBox *box = new QGroupBox(tr("数据采集"));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(testView,0,0);
    box->setLayout(layout);
    return box;
}

QGroupBox* mydb::creatDetailGroup()
{
    QGroupBox *box = new QGroupBox(tr("详细信息"));
    showList = new QListWidget;
    QPushButton *deleteBtn = new QPushButton(tr("删除"));
    QPushButton *addBtn = new QPushButton(tr("告警"));
    QPushButton *selectBtn = new QPushButton(tr("查找"));
    QPushButton *returnBtn = new QPushButton(tr("返回列表"));
    QPushButton *quitBtn = new QPushButton(tr("退出"));
    seLineEdit = new QLineEdit;
    QGridLayout *layout = new QGridLayout;
    connect(deleteBtn,SIGNAL(clicked(bool)),this,SLOT(removeTestFromTestModel()));
    connect(selectBtn,SIGNAL(clicked(bool)),this,SLOT(selectSlot()));
    connect(returnBtn,SIGNAL(clicked(bool)),this,SLOT(returnSlot()));
    connect(addBtn,SIGNAL(clicked(bool)),this,SLOT(addNewSlot()));
    connect(quitBtn,SIGNAL(clicked(bool)),this,SLOT(quitBtnSlot()));

    layout->addWidget(showList,0,0,3,2);
    layout->addWidget(quitBtn,5,1,1,1);
    layout->addWidget(addBtn,2,0,3,2);
    layout->addWidget(seLineEdit,4,0,1,1);
    layout->addWidget(selectBtn,4,1,1,1);
    layout->addWidget(deleteBtn,5,0,1,1);
    layout->addWidget(returnBtn,3,0,3,2);
    box->setLayout(layout);
    return box;

}
void mydb::changeTestView(QModelIndex index)
{
    showList->clear();
    item = new QListWidgetItem(showList);
    QSqlRecord record = testModel->record(index.row());
    int route = record.value("route").toInt();
    int addr =record.value("addr").toInt();
    int num = record.value("num").toInt();
    int ttyO1 =record.value("ttyO1").toInt();
    QString myttyO1;
    if(ttyO1 == 0)
    {
    myttyO1 = "fail";
    }
    else
    myttyO1 = "pass";
    int ttyO2 = record.value("ttyO2").toInt();
    QString myttyO2;
    if(ttyO2 == 0)
    {
    myttyO2 = "fail";
    }
    else
    myttyO2 = "pass";
    int ttyO3 =record.value("ttyO3").toInt();
    QString myttyO3;
    if(ttyO3 == 0)
    {
    myttyO3 = "fail";
    }
    else
    myttyO3 = "pass";
    int ttyO4 = record.value("ttyO4").toInt();
    QString myttyO4;
    if(ttyO4 == 0)
    {
    myttyO4 = "fail";
    }
    else
    myttyO4 = "pass";
    int ttyO5 = record.value("ttyO5").toInt();
    QString myttyO5;
    if(ttyO5 == 0)
    {
    myttyO5 = "fail";
    }
    else
    myttyO5 = "pass";
    int USB =record.value("USB").toInt();
    QString myUSB;
    if(USB == 0)
    {
    myUSB = "fail";
    }
    else
    myUSB = "pass";
    int NET = record.value("NET").toInt();
    QString myNET;
    if(NET == 0)
    {
    myNET = "fail";
    }
    else
    myNET = "pass";
    int RTC = record.value("RTC").toInt();
    QString myRTC;
    if(RTC == 0)
    {
    myRTC = "fail";
    }
    else
    myRTC = "pass";
    QString mytime = record.value("CreatedTime").toString();

    item->setText(tr("route：%1\n addr：%2\n num：%3\n ttyO1：%4\n ttyO2：%5\n ttyO3：%6\n ttyO4：%7\n ttyO5：%8\n USB：%9\n NET：%10\n RTC：%11\n CreatedTime：%12\n"\
        ).arg(route).arg(addr).arg(num).arg(myttyO1).arg(myttyO2).arg(myttyO3).arg(myttyO4).arg(myttyO5).arg(myUSB).arg(myNET).arg(myRTC).arg(mytime));
}
/* 删除 */
void mydb::removeTestFromTestModel()
{
    QModelIndexList select = testView->selectionModel()->selectedRows(0);
    if(!select.empty())
    {
        QModelIndex idIndex = select.at(0);
        QString test = idIndex.sibling(idIndex.row(),1).data().toString();
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,tr("删除信息"),QString (tr("确定从数据库删除'%1'的信息吗？").arg(test)),QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            testModel->removeRow(idIndex.row());
            testModel->submitAll();
            testModel->select();
        }
        else
        {
            QMessageBox::information(this,tr("删除信息"),tr("请选择要删除的信息！"));
        }
    }
}
/* 查找 */
void mydb::selectSlot()
{
    QString test = seLineEdit->text();
    if(test.isEmpty())
        return;
    else
    {
        testModel->setFilter(QObject::tr("addr = '%1'").arg(test));
        testModel->select();
        seLineEdit->clear();
    }
}
/* 返回列表 */
void mydb::returnSlot()
{
    testModel->setTable("test");
    qDebug()<<"testing";

    testModel->setSort(0,Qt::AscendingOrder);
    testModel->select();
}/* 退出 */
void mydb::quitBtnSlot()
{
    //emit sendsignal();
    this->close();
}
/* 警告 */
void mydb::addNewSlot()
{
    /*
    QString test = "1";
    testModel->setFilter(QObject::tr("ttyO5 = '%1'").arg(test));

    //QString test = "0";
    //testModel->setFilter(QObject::tr(" ttyO1 LIKE '%1' OR ttyO2 LIKE '%1' OR ttyO3 LIKE '%1' OR ttyO4 LIKE '%1'\
    OR ttyO5 LIKE '%1' OR USB LIKE '%1' OR NET LIKE '%1' OR RTC LIKE '%1'").arg(test));

    //QSqlQuery *str;
    //str = "SELECT test, ttyO5 FROM test ORDER BY ttyO5";
    //testModel->setQuery(&str);


    tetModel = new QSqlQueryModel(this);
    tetModel->setQuery("SELECT route FROM test ORDER BY test");
    if(tetModel->lastError().isValid())
    {
        QMessageBox::critical(this,"错误",\
                              "数据表查询错误，错误信息\n"+tetModel->lastError().text(),
                              QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }
    */

    testModel->setFilter("ttyO5='1'");//查找ttyO5数据为0
    QSqlRecord record = testModel->record(1);
    int route = record.value("route").toInt();
    int addr =record.value("addr").toInt();
    qDebug()<< "route = "<<route;
    qDebug()<< "addr = "<<addr;
    //emit send_db(route,addr);
}
int mydb::result_fun(int line)
{
    QSqlRecord record = testModel->record(line);
    int route2 = record.value("route").toInt();
    int addr2 =record.value("addr").toInt();
    qDebug()<< "route2 = "<<route2;
    qDebug()<< "addr2 = "<<addr2;
    //已经转到完成一整轮

    //之后就要判断这一行的数据是什么
    //之后的就应该是就应该循环

    int ttyO1 =record.value("ttyO1").toInt();
    int ttyO2 = record.value("ttyO2").toInt();
    int ttyO3 =record.value("ttyO3").toInt();
    int ttyO4 = record.value("ttyO4").toInt();
    int ttyO5 = record.value("ttyO5").toInt();
    int RS485 = ttyO1 + ttyO2 + ttyO3 + ttyO4 + ttyO5;

    int USB =record.value("USB").toInt();
    int NET = record.value("NET").toInt();
    int RTC = record.value("RTC").toInt();
    int result = RTC + NET + USB + RS485;    //总的结果
    return result;

}
/* 发送次数 */
void mydb::send_num_fun()
{
    testModel->setSort(2,Qt::DescendingOrder);    //数字2表示 测试次数 那一列倒序      //根据num来
    testModel->select();
    QSqlRecord record = testModel->record(0);     //已倒序
    int num = record.value("num").toInt();
    num = num/16;
    emit send_num(num);

}
/* 发送到主窗口route addr */
void mydb::send_db_fun()
{
    //数据库发送的那端 可以判断所有的数据相加是不是等于8
    //从数据库中查找ttyO5为1的
    //testModel->setFilter("ttyO5='1'");//查找ttyO5数据为1
    testModel->setSort(2,Qt::DescendingOrder);    //数字2表示 测试次数 那一列倒序      //根据num来
    testModel->select();
    //这里可以循环16次 表示每一个

    QSqlRecord record = testModel->record(0);     //已倒序
    //判断重新倒序后的这一行是哪一行
    int product = 0;
    int route = record.value("route").toInt();
    int addr =record.value("addr").toInt();
    qDebug()<< "route = "<<route;
    qDebug()<< "addr = "<<addr;

    switch(route)
    {
        case 1:
            switch(addr)
            {
                case 1:product =1;                break;
                case 2:product =2;                break;
                case 3:product =3;                break;
                case 4:product =4;                break;

            }
            break;
        case 2:
            switch(addr)
            {
                case 1:product =5;                break;
                case 2:product =6;                break;
                case 3:product =7;                break;
                case 4:product =8;                break;
                break;
            }
            break;
        case 3:
            switch(addr)
            {
                case 1:product =9;                break;
                case 2:product =10;                break;
                case 3:product =11;                break;
                case 4:product =12;                break;
            }
            break;
        case 4:
            switch(addr)
            {
                case 1:product =13;                break;
                case 2:product =14;                break;
                case 3:product =15;                break;
                case 4:product =16;                break;
            }
            break;
    }
    qDebug()<< "product = "<< product;//可以得知是已倒序的那个产品

    int tmp = 16 - product;
    int tmp2 = 16 - tmp;

    int result16 = result_fun(tmp2);
    int result15 = result_fun(tmp2+1);
    int result14 = result_fun(tmp2+2);
    int result13 = result_fun(tmp2+3);
    int result12 = result_fun(tmp2+4);
    int result11 = result_fun(tmp2+5);
    int result10 = result_fun(tmp2+6);
    int result9 = result_fun(tmp2+7);
    int result8 = result_fun(tmp2+8);
    int result7 = result_fun(tmp2+9);
    int result6 = result_fun(tmp2+10);
    int result5 = result_fun(tmp2+11);
    int result4 = result_fun(tmp2+12);
    int result3 = result_fun(tmp2+13);
    int result2 = result_fun(tmp2+14);
    int result1 = result_fun(tmp2+15);


    qDebug()<<"result1 = "<< result1;
    qDebug()<<"result2 = "<< result2;
    qDebug()<<"result3 = "<< result3;
    qDebug()<<"result4 = "<< result4;
    qDebug()<<"result5 = "<< result5;
    qDebug()<<"result6 = "<< result6;
    qDebug()<<"result7 = "<< result7;
    qDebug()<<"result8 = "<< result8;
    qDebug()<<"result9 = "<< result9;
    qDebug()<<"result10 = "<< result10;
    qDebug()<<"result11 = "<< result11;
    qDebug()<<"result12 = "<< result12;
    qDebug()<<"result13 = "<< result13;
    qDebug()<<"result14 = "<< result14;
    qDebug()<<"result15 = "<< result15;
    qDebug()<<"result16 = "<< result16;


    //不需要传设备编号 只需要传结果 因为你知道每个图像对应的位置
    //这样就得直接固定 查的是倒序后的 测完一轮的第一个或者最后一个
    //向主窗口发送信号    //16个
    emit send_db(result1,result2,result3,result4,result5,result6,result7,result8,result9,result10,result11,\
                 result12,result13,result14,result15,result16);//所以现在不需要发送给主界面route addr 直接一个product
    //发送信号 按照一个一个的发 这样应该是

    //需要一次性发送过去
    qDebug()<< "send_db_fun emit";
}
void mydb::send_ttyO5_fun()
{
    //可以根据测试时间或者测试次数在数据库中提取
    //将时间或者次数倒序
    //根据时间无法判定是那个 设备的数据 所以需要根据num来

    testModel->setSort(11,Qt::DescendingOrder);    //数字11表示 测试时间 那一列        //倒序
    testModel->select();
    QSqlRecord record = testModel->record(1);

    int route = record.value("route").toInt();
    int addr =record.value("addr").toInt();

    int ttyO1 =record.value("ttyO1").toInt();
    int ttyO2 = record.value("ttyO2").toInt();
    int ttyO3 =record.value("ttyO3").toInt();
    int ttyO4 = record.value("ttyO4").toInt();
    int ttyO5 = record.value("ttyO5").toInt();
    int RS485 = ttyO1 + ttyO2 + ttyO3 + ttyO4 + ttyO5;

    int USB =record.value("USB").toInt();

    int NET = record.value("NET").toInt();

    int RTC = record.value("RTC").toInt();

    qDebug()<< "route = "<<route;
    qDebug()<< "addr = "<<addr;


    //写到一个数组里

    //向主窗口发送信号
    emit send_ttyO5(ttyO5);
    qDebug()<< "send_ttyO5_fun emit";
}
mydb::~mydb()
{

}
