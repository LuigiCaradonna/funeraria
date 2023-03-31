#include "funeraria.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Funeraria w;
    w.show();
    return a.exec();
}
