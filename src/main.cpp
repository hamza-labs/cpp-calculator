#include "calculatrice.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculatrice c;
    c.show();

    return a.exec();
}
