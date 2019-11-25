#ifndef DBCONNECTER_H
#define DBCONNECTER_H

#include <QSqlDatabase>
#include <QScopedPointer>

#include "pgworker.h"

class DBConnecter
{

public:
    DBConnecter();

    static bool         connect() noexcept;
    static bool         readFunctions() noexcept;

    static PGWorker *   createWorker() noexcept;


private:

    static inline QSqlDatabase _db;

    static inline PGWorker * _worker = nullptr;

};

#endif // DBCONNECTER_H
