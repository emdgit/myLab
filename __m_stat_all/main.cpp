#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonDocument>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include <iostream>

#include "chart.h"
#include "config.h"
#include "connecter.h"
#include "pnode.h"
#include "coreapi.h"
#include "modelmanager.h"
#include "hintmodel.h"
#include "storage.h"
#include "purchasegroupmodel.h"
#include "signalmanager.h"
#include "period.h"

using namespace std;

void readConfigFile();
void readDBConfig(QJsonObject &dbObject);

static QObject * core_api_singleton_f(QQmlEngine *qml, QJSEngine * js)
{
    Q_UNUSED(qml)
    Q_UNUSED(js)

    return new CoreAPI;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qRegisterMetaType<PNodeIndex>( "PNodeIndex" );

    qmlRegisterType<Chart>( "OwlComponents", 1, 0, "Chart" );
    qmlRegisterType<PurchaseGroupModel>( "OwlComponents", 1, 0, "PGroupModel" );
    qmlRegisterSingletonType<CoreAPI>( "OwlComponents", 1, 0, "CoreAPI", core_api_singleton_f );
    qmlRegisterUncreatableType<HintModel>( "OwlComponents", 1, 0, "HintModel",
                                         "HintModel is an uncreatable type" );
    qmlRegisterUncreatableType<Period>("OwlComponents", 1, 0, "Period",
                                       "Period is an uncreatable type");

    // Хранилище доходных групп
    auto stProfit = ST.groupsProfit();

    // Хранилище расходных групп
    auto stSpend = ST.groupsSpend();

    auto hints = new HintModel();

    // Менеджер моделей
    ModelManager mmanager;
    mmanager.setSpendModel( new PurchaseGroupModel(stSpend) );
    mmanager.setProfitModel( new PurchaseGroupModel(stProfit) );
    mmanager.setHintModel( hints );
    mmanager.setPurchaseModelDaily(new PurchaseModelDaily());
    mmanager.setPeriodModel(new PeriodModel());

    // Менеджер сигналов
    SignalManager smanager;

    // Установить хранилища
    CoreAPI::initSpendGroupCallback();
    CoreAPI::initProfitGroupCallback();
    CoreAPI::setModelManager( &mmanager );
    CoreAPI::setSignalManager( &smanager );

    try {
        readConfigFile();
    }
    catch ( const exception &ex ) {
        cout << ex.what() << endl;
        return -1;
    }

    auto t1 = chrono::high_resolution_clock::now();

    if ( !pg::Connecter::readFunctions() ) {
        qDebug() << "Error occured while reading functions";
        return -2;
    }
    else {
        // Загрузить все группы расходов/доходов
        CoreAPI::loadGroups( false );
        CoreAPI::loadGroups( true );

        // Загрузить все записи расходов/доходов
        CoreAPI::loadRecords( false );
        CoreAPI::loadRecords( true );

        // Модель подсказок по умолчанию показывает расходы
        CoreAPI::switchHintModel(false);

        // Задать пользователя и его группу. (DEV version) // todo
        CoreAPI::setCurrentUser(1,1);

        // Установить начало периода // todo
        if (!CoreAPI::hasStartPoint()) {
            CoreAPI::setStartPoint(QDate(2019,5,11));
        }

        // Загрузить покупки текущего периода
        CoreAPI::loadPurchasesSumm(false);
        CoreAPI::loadPurchasesSumm(true);
        CoreAPI::loadPurchases(false);
        CoreAPI::loadPurchases(true);
    };

    auto t2 = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> d(t2-t1);

    cout << "Loaded at " << d.count() << "ms" << endl;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty( "ModelManager", &mmanager );
    engine.rootContext()->setContextProperty( "SignalManager", &smanager );
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    if (engine.rootObjects().isEmpty())
        return -3;

    return app.exec();
}

void readConfigFile()
{
    constexpr const char * config_file = "config.json";

    if ( !QFileInfo(config_file).exists() ) {
        QFile def_conf(":/conf/config.json");
        QFile tar_conf("config.json");

        def_conf.open(QIODevice::ReadOnly);
        tar_conf.open(QIODevice::WriteOnly);

        QTextStream in(&def_conf);
        QTextStream out(&tar_conf);

        auto conf = in.readAll();
        out << conf;

        def_conf.close();
        tar_conf.close();
    }

    QFile f( config_file );

    if ( !f.open( QIODevice::ReadOnly ) ) {
        throw runtime_error("Cannot open 'config.json' for read");
    }

    QTextStream stream( &f );

    auto conf_data = stream.readAll();

    f.close();

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson( conf_data.toLatin1(), &err );

    if ( err.error != QJsonParseError::NoError ) {
        throw runtime_error("Invalid configuration!");
    }

    if ( !doc.isObject() ) {
        throw runtime_error("Invalid configuration structure!");
    }

    auto obj = doc.object();
    auto dbObject = obj.value( "DB" ).toObject();

    readDBConfig( dbObject );
}

void readDBConfig(QJsonObject &dbObject)
{
    bool valid_port_value;
    auto name = dbObject.value( "Name" ).toString();
    auto host = dbObject.value( "Host" ).toString();
    auto pswd = dbObject.value( "Password" ).toString();
    auto user = dbObject.value( "User" ).toString();
    auto port = dbObject.value( "Port" ).toString().toInt(&valid_port_value);

    // При корректном конфиге каждая проверка должна вернуть false
    auto valid_config = !(name.isEmpty() ||
                          host.isEmpty() ||
                          pswd.isEmpty() ||
                          user.isEmpty() );

    if ( !valid_config || !valid_port_value ) {
        throw runtime_error("Invalid 'DB' section in configuration file!");
    }

    pg::Config::dbName = name;
    pg::Config::dbHost = host;
    pg::Config::dbPort = port;
    pg::Config::dbPswd = pswd;
    pg::Config::dbUser = user;

    if ( !pg::Connecter::connect() ) {
        throw runtime_error("Cannot connect to DB!");
    }
}
