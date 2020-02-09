#include "worker.h"

#include <QSqlRecord>

class WorkerPrivate
{
public:

    WorkerPrivate(){}

    pg::Answer * extractAnswer( const QSqlQuery &query ) const noexcept
    {
        using namespace pg;

        int len = query.record().count();

        Answer a( len );

        qDebug() << "rows = " << query.size();
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
