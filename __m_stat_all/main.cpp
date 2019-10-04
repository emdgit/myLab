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

int main(int argc, char *argv[])
{
    using namespace std;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Chart>( "OwlComponents", 1, 0, "Chart" );

    qmlRegisterType<TestModel>( "TestModel", 1, 0, "TestModel" );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    set<QString> s1;
    s1.insert( "name" );
    s1.insert( "value" );
    s1.insert( "id" );

    for ( auto i : s1 )
        qDebug() << i;

    qDebug("\n");

    set<shared_ptr<QString>> s2;
    s2.insert( std::make_shared<QString>( "name" ) );
    s2.insert( std::make_shared<QString>( "value" ) );
    s2.insert( std::make_shared<QString>( "id" ) );


    for ( auto i : s2 )
        qDebug() << *i.get();


    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
