#ifndef DBCONFIG_H
#define DBCONFIG_H

#include <QString>

namespace pg {
    struct Config;
}

struct pg::Config
{
    static inline QString   dbName = "";
    static inline QString   dbHost = "";
    static inline QString   dbUser = "";
    static inline QString   dbPswd = "";
    static inline int       dbPort = -1;

    static bool             isFull() noexcept
    {
        return ( !dbName.isEmpty() &&
                 !dbHost.isEmpty() &&
                 !dbUser.isEmpty() &&
                 dbPort > 0 );
    }
};

#endif // DBCONFIG_H
