#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "dbconnecter.h"
#include "dbconfig.h"

static const QString _in_args = "args";
static const QString _out_args = "res";
static const QString _func_name = "proname";
static const QString _schema = "_schema";

DBConnecter::DBConnecter()
{

}

bool DBConnecter::connect() noexcept
{
    if ( !DBConfig::isFull() )
        return false;

    if ( _db.isValid() )
    {
        //  Check if same connection
        if ( _db.isOpen() )
        {
            if ( _db.databaseName() == DBConfig::dbName &&
                 _db.hostName() == DBConfig::dbHost &&
                 _db.port() == DBConfig::dbPort &&
                 _db.userName() == DBConfig::dbUser &&
                 _db.password() == DBConfig::dbPswd )
                return false;

            _db.close();
        }

        QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
    }

    _db = QSqlDatabase::addDatabase( "QPSQL" );
    _db.setHostName( DBConfig::dbHost );
    _db.setPort( DBConfig::dbPort );
    _db.setDatabaseName( DBConfig::dbName );
    _db.setUserName( DBConfig::dbUser );
    _db.setPassword( DBConfig::dbPswd );

    return _db.open();
}

bool DBConnecter::readFunctions() noexcept
{
    if ( !_db.isValid() || !_db.isOpen() )
        return false;

    QFile f( ":/sql/Func_Info.sql" );

    if ( !f.open( QIODevice::ReadOnly ) )
        return false;

    QString queryStr( f.readAll() );
    QSqlQuery query;

    query.prepare( queryStr );
    query.exec();

    if ( query.lastError().isValid() )
        return false;

    while ( query.next() )
    {

    }

    return true;
}
