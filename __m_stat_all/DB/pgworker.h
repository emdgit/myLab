#ifndef PGWORKER_H
#define PGWORKER_H

#include <QSqlDatabase>

#include "pgfunction.h"
#include "pganswer.h"

class PGWorkerPrivate;

class PGWorker
{
public:
    PGWorker( QSqlDatabase * db );

    PGAnswer * execute( const PgFunction &func ) const noexcept;


private:

    QSqlDatabase *  _db = nullptr;

    QScopedPointer<PGWorkerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(PGWorker)

};

#endif // PGWORKER_H
