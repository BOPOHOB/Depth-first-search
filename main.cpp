\#include "gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI w;
    w.setWindowTitle("Сквэрворд ");
    w.show();

    return a.exec();
}