#include "pgworker.h"

class PGWorkerPrivate
{
public:

    PGWorkerPrivate(){}

    PGAnswer * extractAnswer( const QSqlQuery &query ) const noexcept
    {
        Q_UNUSED( query );

        return nullptr; /// \todo
    }

};

PGWorker::PGWorker(QSqlDatabase * db) : _db(db) {}

PGAnswer *PGWorker::execute(const PgFunction & func) const noexcept
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



    return nullptr; /// \todo
}
