#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>

#include <set>

#include "chart.h"
#include "testmodel.h"
#include "config.h"
#include "connecter.h"

#include "function.h"
#include "typestorage.h"
#include "pnode.h"

#include "purchasegroup.h"

int main(int argc, char *argv[])
{
    using namespace std;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

//    QGuiApplication app(argc, argv);

    qRegisterMetaType<PNodeIndex>( "PNodeIndex" );

    qmlRegisterType<Chart>( "OwlComponents", 1, 0, "Chart" );
    qmlRegisterType<TestModel>( "TestModel", 1, 0, "TestModel" );

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    pg::Config::dbName = "SuperMegaDatabase6000";     //  Some hardcode here,
    pg::Config::dbHost = "127.0.0.1";                 //  I'll place it to GUI...
    pg::Config::dbPort = 5433;                        //  ... later... =)
    pg::Config::dbPswd = "123456qQ";
    pg::Config::dbUser = "postgres";

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

//    if (engine.rootObjects().isEmpty())
//        return -1;

//    return app.exec();
    return 0;
}
