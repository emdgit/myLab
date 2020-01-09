#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>

#include "chart.h"
#include "testmodel.h"
#include "config.h"
#include "connecter.h"

#include "function.h"
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
    pg::Config::dbName = "purchase_db";
    pg::Config::dbHost = "127.0.0.1";
    pg::Config::dbPort = 5432;
    pg::Config::dbPswd = "123456qQ";
    pg::Config::dbUser = "postgres";
#else
    pg::Config::dbName = "SuperMegaDatabase6000";     //  Some hardcode here,
    pg::Config::dbHost = "127.0.0.1";                 //  I'll place it to GUI...
    pg::Config::dbPort = 5433;                        //  ... later... =)
    pg::Config::dbPswd = "123456qQ";
    pg::Config::dbUser = "postgres";
#endif

    auto b = pg::Connecter::connect();

    if ( b )
    {
        qDebug() << "Connected to DataBase" << pg::Config::dbName;
        pg::Connecter::readFunctions();
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

        auto w = pg::Connecter::createWorker();
        w->execute( *func );
    }
    else
    {
        qDebug() << "Func hasn't found";
    }



    return app.exec();
}
