#ifndef MYREGISTER_H
#define MYREGISTER_H

#include <QMainWindow>

namespace Ui {
class myregister;
}

class myregister : public QMainWindow
{
    Q_OBJECT

public:
    explicit myregister(QWidget *parent = 0);
    ~myregister();

private slots:
    void on_enter_button_clicked();

    void on_quit_button_clicked();

private:
    Ui::myregister *ui;
};

#endif // MYREGISTER_H
