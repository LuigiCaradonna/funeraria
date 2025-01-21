#include "funeraria.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Funeraria w;
    // If the app initialization failed, quit the program
    if (!w.up) { a.quit(); return -1; }

    w.setStyle(QStyleFactory::create("WindowsVista"));
    w.showMaximized();
    w.show();
    return a.exec();
}
