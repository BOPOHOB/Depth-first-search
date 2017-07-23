#include "gui.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator* const translator(new QTranslator(&a));
    translator->load(":/ru.qm");
    a.installTranslator(translator);

    GUI w;
    w.show();

    return a.exec();
}
