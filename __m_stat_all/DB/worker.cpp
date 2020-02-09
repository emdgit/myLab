#include "worker.h"

#include <QSqlRecord>
#include <QSqlField>

class WorkerPrivate
{
public:

    WorkerPrivate(){}

    pg::Answer * extractAnswer( QSqlQuery &query ) const noexcept
    {
        using namespace pg;

        int len = query.record().count();

        auto ans = new Answer( len );

        while ( query.next() )
        {
            auto rec = query.record();

            for ( int i(0); i < len; ++i )
            {
                auto f = rec.field( i );
                ans->insertValue( f.name(), f.value() );
            }
        }

        if ( ans->isValid() )
            return ans;

        delete ans;
        return nullptr;
    }

};

pg::Worker::Worker(QSqlDatabase * db) : _db(db) {}

pg::Answer *pg::Worker::execute(const pg::Function &func) const noexcept
{
    if ( !_db )
        return nullptr;

    QSqlQuery query( *_db );

    func.prepare( query );

    const bool success = query.exec();

    if ( !success )
    {
        return nullptr;
    }

    Q_D ( const Worker );

    return d->extractAnswer( query );
}
