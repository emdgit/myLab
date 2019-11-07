#ifndef DBCONNECTER_H
#define DBCONNECTER_H

#include <QSqlDatabase>

class DBConnecter
{

public:
    DBConnecter();

    static bool     connect() noexcept;


private:

    static inline QSqlDatabase _db;

};

#endif // DBCONNECTER_H
