#ifndef MYSETTING_H
#define MYSETTING_H

#include <QMainWindow>
#include <QGridLayout>

#include <QGroupBox>
#include <QSqlTableModel>
#include <QTableView>
#include <QListWidget>
#include <QLineEdit>
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>


class mysetting : public QMainWindow
{
    Q_OBJECT
public:
    explicit mysetting(QWidget *parent = 0);
    ~mysetting();


signals:
    void send_str_end(QString str,QString end);

public slots:
    void quit_func();
    void enter_func();

private:
    QGroupBox *timebox;
    QGroupBox *numberbox;
    QGridLayout *layout;
    QWidget *widget;

    QGroupBox* createTimeCroup();
    QGroupBox* creatNumberGroup();


        QComboBox *downbox;
        QComboBox *downbox_1;
    //QDialog * newDialog;


};

#endif // MYSETTING_H
