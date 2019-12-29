#ifndef MYDB_H
#define MYDB_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QGroupBox>
#include <QSqlTableModel>
#include <QTableView>
#include <QListWidget>
#include <QLineEdit>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QSqlRecord>
#include <QLabel>
#include <QMessageBox>

#include <QDataWidgetMapper>//数据映射

class mydb : public QMainWindow
{
    Q_OBJECT
public:
    //explicit mydb(QWidget *parent = 0);
    //mydb(const QString &testTable,QDialog *parent = 0);
    explicit mydb(QDialog *parent = 0);
    void setHeader();

    //void send_db_fun();

    QSqlTableModel *testModel;
    int result_fun(int line);

    ~mydb();
public slots:
    void changeTestView(QModelIndex index);
    void removeTestFromTestModel();
    void selectSlot();
    void returnSlot();
    void quitBtnSlot();
    void addNewSlot();

    void send_db_fun();
    void send_ttyO5_fun();
    void send_num_fun();







signals:
    void send_db(int result1,int result2,int result3,int result4,int result5,int result6,int result7,int result8,\
                 int result9,int result10,int result11,int result12,int result13,int result14,int result15,int result16);
    void send_ttyO5(int ttyO5);
    void send_num(int num);

private:
    QGroupBox *createTestCroup();
    QGroupBox *creatDetailGroup();

    QTableView *testView;
    QListWidget *showList;
    QLineEdit *seLineEdit;
    QListWidgetItem *item;
    QLineEdit *idLineEdit;

    QSqlQueryModel *tetModel;//数据模型
    QDataWidgetMapper *dataMapper;//数据映射



};

#endif // MYDB_H
