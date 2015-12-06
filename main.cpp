#include "chatserverwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatServerWidget w;
    w.show();

    return a.exec();
}
