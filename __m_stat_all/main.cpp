#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>

#include <set>
#include <iostream>

#include "chart.h"
#include "testmodel.h"
#include "dbconfig.h"
#include "dbconnecter.h"
#include "exception.h"
#include "typestorage.h"

using namespace Owl::Exception;

struct A {

    [[noreturn]] void f()
    {
        throw makeException( this, "vot tak vot!" );
    }
};

int main(int argc, char *argv[])
{
    using namespace std;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    std::shared_ptr<QString> sptr;
    sptr = std::make_shared<QString>( "QQQQQ" );
    auto opt1 = std::make_optional<std::shared_ptr<QString>>( sptr );


    TypeStorage::registerField( "QQ" );
    auto f = TypeStorage::field( "QQ" );
    auto f1 = TypeStorage::field( "QQ" );










A aa;
    try {
        aa.f();
    }
    catch ( Exception<std::decay_t<A>> & ex ) {
        cout << "\n" << ex.what() << endl;
    }
    catch ( Exception<int> & ex ) {
        cout << ex.what() << endl;
    }

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
