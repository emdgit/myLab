#ifndef DBCONNECTER_H
#define DBCONNECTER_H

#include <QSqlDatabase>

#include "worker.h"

namespace pg {
    class Connecter;
}

class pg::Connecter
{

public:
    Connecter();

    static bool         connect() noexcept;
    static bool         readFunctions() noexcept;

    static pg::Worker * createWorker() noexcept;


private:

    static inline QSqlDatabase _db;

    static inline pg::Worker * _worker = nullptr;

};

#endif // DBCONNECTER_H
