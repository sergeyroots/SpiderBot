#include "spbmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    spbMainWindow w;
    w.show();

    return a.exec();
}
