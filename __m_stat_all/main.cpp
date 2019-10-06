#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>

#include <set>

#include "chart.h"
#include "field.h"
#include "templates.h"
#include "testmodel.h"
#include "head.h"
#include "Set.h"

int main(int argc, char *argv[])
{
    using namespace std;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Chart>( "OwlComponents", 1, 0, "Chart" );

    qmlRegisterType<TestModel>( "TestModel", 1, 0, "TestModel" );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    MPSet<int> s1;
    s1.insert( make_shared<int>(4) );
    s1.insert( make_shared<int>(5) );
    s1.insert( make_shared<int>(1) );
    s1.insert( make_shared<int>(-4) );
    s1.insert( make_shared<int>(4) );

    for ( const auto &i : s1 )
        qDebug() << *i.get();


    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
