#include <QCoreApplication>

#include "guardiancore.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GuardianCore gc(a.parent());

    return a.exec();
}
