#include "wifibot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wifibot w;

    w.show();

    return a.exec();
}
