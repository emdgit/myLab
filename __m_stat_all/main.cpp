#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "mobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    auto mo = new MObject();
    mo->randomIcon();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    engine.rootContext()->setContextProperty( "mObject", mo );


    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
