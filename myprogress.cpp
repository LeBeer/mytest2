#include "myprogress.h"
#include <qdir.h>
#include <qdebug.h>

myprogress::myprogress(QObject* parent) : QProcess(parent)
{/* 启动485测试程序 */
    path = QDir::currentPath()+"/test";
    qDebug(path.toLatin1());
    start(path);
}

myprogress::~myprogress()
{

}

