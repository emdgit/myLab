#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>

#include "dbconnecter.h"
#include "dbconfig.h"
#include "pgfunction.h"
#include "exception.h"
#include "typestorage.h"

static const QString _in_args = "args";
static const QString _out_args = "res";
static const QString _func_name = "proname";
static const QString _schema = "_schema";

static const QString _in_arg_rx = "^(?P<name>\\S+)\\s(?P<type>.+?)(($)|(\\sDEFAULT\\s"
                                  "(?<default_value>.+)::(?<default_value_type>.+)$))";

class DBConnecterPrivate
{

    using _FIA = FuncInArgument;

public:
    DBConnecterPrivate(){}

    static void readInArguments( const QString &&in, PgFunction &f )
    {
        if ( in.isEmpty() )
            return;

        auto lstArgs = in.split( ",", QString::SkipEmptyParts );

        QRegularExpression r( _in_arg_rx );
        auto capGroups = r.namedCaptureGroups();

        for ( const auto &arg : lstArgs )
        {
            auto match = r.match( arg );

            if ( match.hasMatch() )
            {
                _FIA inArg;

                auto name = match.captured( "name" );
                auto type = match.captured( "type" );
                auto defVal = match.captured( "default_value" );
                auto defValType = match.captured( "default_value_type" );

                if ( !defValType.isEmpty() )
                {
                    if ( defValType != type )
                        throw makeException<DBConnecterPrivate>( std::string( "Wrong arg: " ) + arg.toStdString() );

                    inArg.isDefault = true;
                }

                auto fld = TypeStorage::field( name );

                if ( !fld )
                {
                    TypeStorage::registerField( name );

                    fld = TypeStorage::field( name );

                    if ( !fld )
                        throw makeException<DBConnecterPrivate>( std::string( "Cannot register field: " ) +
                                                                 name.toStdString() );
                }

                inArg.field = (*fld);
                f.addIn( std::forward<_FIA>( inArg ) );
            }
        }
    }

};

typedef DBConnecterPrivate D;

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
        PgFunction func;

        auto str = query.record().field( _in_args ).value().toString();

        D::readInArguments( std::move( str ), func );

        qDebug() << str;
    }

    return true;
}
