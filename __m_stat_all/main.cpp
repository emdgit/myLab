#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>

#include <set>

#include "chart.h"
#include "testmodel.h"
#include "dbconfig.h"
#include "dbconnecter.h"

int main(int argc, char *argv[])
{
    using namespace std;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Chart>( "OwlComponents", 1, 0, "Chart" );
    qmlRegisterType<TestModel>( "TestModel", 1, 0, "TestModel" );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    DBConfig::dbName = "purchase_db";
    DBConfig::dbHost = "127.0.0.1";
    DBConfig::dbPort = 5432;
    DBConfig::dbPswd = "123456qQ";
    DBConfig::dbUser = "postgres";

    auto b = DBConnecter::connect();

    if ( b )
    {
        DBConnecter::readFunctions();
    }

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
