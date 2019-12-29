#ifndef MYPROGRESS_H
#define MYPROGRESS_H
#include <qprocess.h>

class myprogress : public QProcess
{
public:

    explicit myprogress(QObject* parent = 0);
    ~myprogress();
private:
    QString path;
};

#endif // MYPROGRESS_H
