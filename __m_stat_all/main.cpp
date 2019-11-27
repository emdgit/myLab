#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>

#include "chart.h"
#include "testmodel.h"
#include "dbconfig.h"
#include "dbconnecter.h"

#include "pgfunction.h"
#include "typestorage.h"

#define HOME

int main(int argc, char *argv[])
{
    using namespace std;
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Chart>( "OwlComponents", 1, 0, "Chart" );
    qmlRegisterType<TestModel>( "TestModel", 1, 0, "TestModel" );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

#ifndef HOME
    DBConfig::dbName = "purchase_db";
    DBConfig::dbHost = "127.0.0.1";
    DBConfig::dbPort = 5432;
    DBConfig::dbPswd = "123456qQ";
    DBConfig::dbUser = "postgres";
#else
    DBConfig::dbName = "SuperMegaDatabase6000";     //  Some hardcode here,
    DBConfig::dbHost = "127.0.0.1";                 //  I'll place it to GUI...
    DBConfig::dbPort = 5433;                        //  ... later... =)
    DBConfig::dbPswd = "123456qQ";
    DBConfig::dbUser = "postgres";
#endif

    auto b = DBConnecter::connect();

    if ( b )
    {
        qDebug() << "Connected to DataBase" << DBConfig::dbName;
        DBConnecter::readFunctions();
    }
    else
    {
        qDebug( "Cannot open DataBase" );
    }

    if (engine.rootObjects().isEmpty())
        return -1;

    auto funcOpt = TypeStorage::func( "get_root_groups", "common" );

    if ( funcOpt )
    {
        auto func = (*funcOpt).get();
        qDebug() << "Func found:" << func->schema() << "." << func->name();

        auto w = DBConnecter::createWorker();
        w->execute( *func );
    }
    else
    {
        qDebug() << "Func hasn't found";
    }



    return app.exec();
}
