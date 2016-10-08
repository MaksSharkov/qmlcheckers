#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlEngine>

#include "checkers/cell.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<Cell>("Checkers",1,0,"Cell");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/qml/view/main.qml")));

    return app.exec();
}
