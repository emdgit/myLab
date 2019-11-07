#include "dbconnecter.h"
#include "dbconfig.h"

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
