#include "logininterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginInterface w;
    w.show();

    return a.exec();
}
