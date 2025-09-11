#include "table.h"
#include "mytime.h"
#include "settingswindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    Table* w = new Table(); w->show();
    MyTime* t = new MyTime(); t->show();

    return a.exec();
}
