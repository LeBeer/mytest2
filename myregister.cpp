#include "myregister.h"
#include "ui_myregister.h"

myregister::myregister(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::myregister)
{
    ui->setupUi(this);
}

myregister::~myregister()
{
    delete ui;
}

void myregister::on_enter_button_clicked()
{

}

void myregister::on_quit_button_clicked()
{
    this->close();

}
