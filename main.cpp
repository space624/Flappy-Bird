#include "gamewindow.h"

#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //将时间设置为随机种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    GameWindow w;
    w.show();
    return a.exec();
}
