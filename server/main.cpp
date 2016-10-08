#include <QCoreApplication>

#include "server.h"
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(QHostAddress::AnyIPv4);
    Q_UNUSED(server)

    return a.exec();
}
