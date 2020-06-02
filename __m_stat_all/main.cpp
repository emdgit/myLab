#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>

#include "chart.h"
#include "config.h"
#include "connecter.h"
#include "pnode.h"
#include "coreapi.h"

using namespace std;

bool connectToDB() noexcept;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Хранилище доходных групп
    PGStorage stProfit;

    // Хранилище расходных групп
    PGStorage stSpend;

    // Установить хранилища
    CoreAPI::setSpendGroupSt( &stSpend );
    CoreAPI::setProfitGroupSt( &stProfit );

    qRegisterMetaType<PNodeIndex>( "PNodeIndex" );

    qmlRegisterType<Chart>( "OwlComponents", 1, 0, "Chart" );

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if ( connectToDB() ) {
        qDebug() << "Connected to DataBase" << pg::Config::dbName;
    }
    else {
        qDebug( "Cannot open DataBase" );
        return -1;
    }

    if ( !pg::Connecter::readFunctions() ) {
        qDebug() << "Error occured while reading functions";
        return -2;
    }

    if (engine.rootObjects().isEmpty())
        return -3;

    return app.exec();
}

bool connectToDB() noexcept
{
    pg::Config::dbName = "SuperMegaDatabase6000";     //  Some hardcode here,
    pg::Config::dbHost = "127.0.0.1";                 //  I'll place it to GUI...
    pg::Config::dbPort = 5433;                        //  ... later... =)
    pg::Config::dbPswd = "123456qQ";
    pg::Config::dbUser = "postgres";

    return pg::Connecter::connect();
}
