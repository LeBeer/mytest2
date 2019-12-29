#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 设置全局字体 */
    int id = QFontDatabase::addApplicationFont("/usr/local/qt5.4.1-enter/lib/fonts/DroidSansFallback.ttf");
    QString msyh = QFontDatabase::applicationFontFamilies(id).at(0);
    qDebug(msyh.toLatin1());
    QFont font(msyh,10);
    font.setPointSize(15);
    a.setFont(font);
    //qApp->setFont(font);

    /* 主窗口 */
    MainWindow w;
    w.show();
    return a.exec();
}
