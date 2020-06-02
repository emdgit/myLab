#ifndef COREAPI_H
#define COREAPI_H

#include <QObject>
#include <QDate>

#include "iobject.h"
#include "storagedefinition.h"

namespace pg {
    class Worker;
}

class CoreAPI : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE
    static bool addPurchase( const int &groupId, const int &userId,
                             const int &recordId, const double &summ,
                             const QDate &date ) noexcept;

    Q_INVOKABLE
    static void loadRootGroups( bool profit = false );

    /// Задать хранилище групп дохода
    static void setProfitGroupSt( PGStorage *st ) noexcept;

    /// Задать хранилище групп расхода
    static void setSpendGroupSt( PGStorage *st ) noexcept;


private:

    static pg::Worker * _pg_worker;

    /// Хранилка групп доходов.
    static PGStorage *  _p_g_storage_profit;

    /// Хранилка групп попупок.
    static PGStorage *  _p_g_storage_spend;

};

#endif // COREAPI_H
