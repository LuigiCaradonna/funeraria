#include "funeraria.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Funeraria w;
    // If the app initialization failed, quit the program
    if (!w.up) { a.quit(); return -1; }
    w.showMaximized();
    w.show();
    return a.exec();
}
