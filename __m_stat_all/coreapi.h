#ifndef COREAPI_H
#define COREAPI_H

#include <QObject>
#include <QDate>

#include <vector>

#include "iobject.h"
#include "storagedefinition.h"

namespace pg {
    class Worker;
    class Answer;
}

class PurchaseRecord;

class CoreAPI : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE
    static bool addPurchase( const int &groupId, const int &userId,
                             const int &recordId, const double &summ,
                             const QDate &date ) noexcept;


    /*!
     * Загрузить корневые группы.
     * В зависимости от переданного параметра profit,
     * загрузка будет либо в хранилище групп доходов,
     * либо групп расходов.
     * \param[in] profit  true - доходы; false - расходы
     */
    static void loadGroups(bool profit = false);


    /*!
     * Загрузить все записи. Конечные единицы товаров.
     * В зависимости от переданного параметра profit,
     * загрузка будет либо в хранилище записей доходов,
     * либо записей расходов.
     * \param[in] profit  true - доходы; false - расходы
     */
    Q_INVOKABLE
    static void loadRecords( bool profit = false );


    /// Задать хранилище групп дохода
    static void setProfitGroupSt( PGStorage *st ) noexcept;


    /// Задать хранилище групп расхода
    static void setSpendGroupSt( PGStorage *st ) noexcept;


protected:

    /// Загрузить группы для данного 'parent', упаковать их в 'st'
    static void loadGroupsByParent( PNodeIndex parent, PGStorage * st );

    /// Разобрать группы из answer и уложить в хранилище под индекс parent
    static void packGroups( pg::Answer * answer, PGStorage * st, PNodeIndex parent );


private:

    static pg::Worker * _pg_worker;

    /// Хранилка групп доходов.
    static PGStorage *  _p_g_storage_profit;

    /// Хранилка групп попупок.
    static PGStorage *  _p_g_storage_spend;

    /// Хранилка доходных записей.
    static inline std::vector<PurchaseRecord*> _records_profit;

    /// Хранилка расходных записей.
    static inline std::vector<PurchaseRecord*> _records_spend;

};

#endif // COREAPI_H
