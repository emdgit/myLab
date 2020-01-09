#ifndef PGWORKER_H
#define PGWORKER_H

#include <QSqlDatabase>

#include "function.h"
#include "answer.h"

class WorkerPrivate;

namespace pg {
    class Worker;
}

class pg::Worker
{
public:
    Worker( QSqlDatabase * db );

    pg::Answer * execute( const pg::Function &func ) const noexcept;


private:

    QSqlDatabase *  _db = nullptr;

    QScopedPointer<WorkerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Worker)

    friend class ::WorkerPrivate;

};

#endif // PGWORKER_H
