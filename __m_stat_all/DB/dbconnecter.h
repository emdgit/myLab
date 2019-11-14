#ifndef DBCONNECTER_H
#define DBCONNECTER_H

#include <QSqlDatabase>
#include <QScopedPointer>

class DBConnecter
{

public:
    DBConnecter();

    static bool     connect() noexcept;
    static bool     readFunctions() noexcept;


private:

    static inline QSqlDatabase _db;

};

#endif // DBCONNECTER_H
