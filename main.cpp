#include "FramelessWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    FramelessWindow w;
    w.show();
    return QApplication::exec();
}