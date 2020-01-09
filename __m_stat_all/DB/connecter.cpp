#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>

#include "connecter.h"
#include "config.h"
#include "function.h"
#include "exception.h"
#include "typestorage.h"

static const QString _in_args = "args";
static const QString _out_args = "res";
static const QString _func_name = "proname";
static const QString _schema = "_schema";

static const QString _arg_rx = "^(?P<name>\\S+)\\s(?P<type>.+?)(($)|(\\sDEFAULT\\s"
                                  "(?<default_value>.+)::(?<default_value_type>.+)$))";

static const QString _if_out_is_table = "^TABLE\\((.+)\\)$";

class ConnecterPrivate
{

    using _FIA = FuncInArgument;

public:
    ConnecterPrivate(){}

    static void readInArguments( const QString &&in, pg::Function &f )
    {
        try {
            readArguments<_FIA>( std::move( in ), f );
        }
        catch ( ... ) {
            throw;
        }
    }

    static void readOutArguments( const QString &&out, pg::Function &f )
    {
        QRegExp rxTable( _if_out_is_table );

        QString str;

        if ( rxTable.indexIn( out ) != -1 )
            str = std::move( std::forward<QString>( rxTable.cap( 1 ) ) );
        else
        {
            TypeStorage::registerField( f.name() );
            auto fld = TypeStorage::field( f.name() );

            if ( !fld )
                throw makeException<ConnecterPrivate>( std::string("cannot register field ") +
                                                       f.name().toStdString() );

            FuncArgument arg;
            arg.field = *fld;

            f.addOut( std::move( arg ) );
            return;
        }

        try {
            readArguments<FuncArgument>( std::move( str ), f );
        }
        catch ( ... ) {
            throw;
        }
    }


private:

    template < typename Arg >
    static void readArguments( const QString &&str, pg::Function &f )
    {
        using namespace Owl::Exception;
        using namespace std;

        static_assert ( std::is_base_of_v< FuncArgument, Arg > );

        constexpr bool isIn = std::is_same_v< _FIA, Arg >;

        if ( str.isEmpty() )
            return;

        auto lstArgs = str.split( ",", QString::SkipEmptyParts );

        for ( const auto &arg : lstArgs )
        {
            try {
                auto inArg = makeArg<Arg>( arg );

                if ( !inArg )
                    throw makeException<ConnecterPrivate>( string( "Wrong Argument: " ) +
                                                           arg.toStdString() );
                if constexpr ( isIn )
                    f.addIn( std::move( *inArg ) );
                else
                    f.addOut( std::move( *inArg ) );
            }
            catch ( ... ) {
                throw;
            }
        }
    }

    template < typename Arg >
    [[nodiscard]] static std::optional<Arg> makeArg( QString str )
    {
        static_assert ( std::is_base_of_v< FuncArgument, Arg > );

        constexpr bool isIn = std::is_same_v< _FIA, Arg >;

        QRegularExpression r( _arg_rx );
        auto capGroups = r.namedCaptureGroups();

        str.remove( QRegExp( "(^\\s+)|(\\s+$)" ) );
        auto match = r.match( str );

        Arg arg;

        if ( match.hasMatch() )
        {
            auto name = match.captured( "name" );
            auto type = match.captured( "type" );
            auto defVal = match.captured( "default_value" );
            auto defValType = match.captured( "default_value_type" );

            if ( !defValType.isEmpty() )
            {
                if ( defValType != type )
                    throw makeException<ConnecterPrivate>( std::string( "Wrong arg: " ) + str.toStdString() );

                if constexpr ( isIn )
                    arg.isDefault = true;
            }

            auto fld = TypeStorage::field( name );

            if ( !fld )
            {
                TypeStorage::registerField( name );

                fld = TypeStorage::field( name );

                if ( !fld )
                    throw makeException<ConnecterPrivate>( std::string( "Cannot register field: " ) +
                                                           name.toStdString() );
            }

            arg.field = (*fld);

            return std::make_optional<Arg>( arg );
        }
        else
            return std::nullopt;
    }

};

typedef ConnecterPrivate D;

pg::Connecter::Connecter() {}

bool pg::Connecter::connect() noexcept
{
    if ( !pg::Config::isFull() )
        return false;

    if ( _db.isValid() )
    {
        //  Check if same connection
        if ( _db.isOpen() )
        {
            if ( _db.databaseName() == pg::Config::dbName &&
                 _db.hostName() == pg::Config::dbHost &&
                 _db.port() == pg::Config::dbPort &&
                 _db.userName() == pg::Config::dbUser &&
                 _db.password() == pg::Config::dbPswd )
                return false;

            _db.close();
        }

        QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
    }

    _db = QSqlDatabase::addDatabase( "QPSQL" );
    _db.setHostName( pg::Config::dbHost );
    _db.setPort( pg::Config::dbPort );
    _db.setDatabaseName( pg::Config::dbName );
    _db.setUserName( pg::Config::dbUser );
    _db.setPassword( pg::Config::dbPswd );

    return _db.open();
}

bool pg::Connecter::readFunctions() noexcept
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
        pg::Function func;

        const auto &&rec = query.record();

        auto str = rec.field( _in_args ).value().toString();
        auto out = rec.field( _out_args ).value().toString();

        func.setSchema( rec.field( _schema ).value().toString() );
        func.setName( rec.field( _func_name ).value().toString() );

        try {
            D::readInArguments( std::move( str ), func );
            D::readOutArguments( std::move( out ), func );
        }
        catch ( Owl::Exception::Exception< std::decay_t<D> > &ex )
        {
            qDebug() << "Error bleat:" << ex.what();
            continue;
        }

        TypeStorage::registerFunc( std::move( func ) );
    }

    return true;
}

pg::Worker *pg::Connecter::createWorker() noexcept
{
    if ( !_worker )
        _worker = new pg::Worker( &_db );

    return _worker;
}
