#ifndef STORAGE_H
#define STORAGE_H

#include <vector>

#include "storagedefinition.h"
#include "singleton.h"
#include "pnode.h"

class PurchaseRecord;
class Purchase;
class Storage;

#define ST Storage::instance()

class Storage : public Singleton<Storage>
{

    using record_vec = std::vector<PurchaseRecord*>;
    using purchase_vec = std::vector<Purchase*>;

public:
    Storage();

    /// Вернуть указатель на хранилище доходных групп
    PGStorage * groupsProfit() noexcept;
    /// Вернуть указатель на хранилище расходных групп
    PGStorage * groupsSpend() noexcept;

    /// Вернуть указатель на хранилище доходных записей
    record_vec *     recordsProfit() noexcept;
    /// Вернуть указатель на хранилище расходных записей
    record_vec *     recordsSpend() noexcept;

    /// Вернуть указатель на хранилище доходных транзакций
    purchase_vec *   purchasesProfit() noexcept;
    /// Вернуть указатель на хранилище расходных транзакций
    purchase_vec *   purchasesSpend() noexcept;


private:

    /// Хранилка групп доходов.
    PGStorage * _p_g_storage_profit;

    /// Хранилка групп попупок.
    PGStorage * _p_g_storage_spend;

    /// Хранилка доходных записей.
    std::vector<PurchaseRecord*> _records_profit;

    /// Хранилка расходных записей.
    std::vector<PurchaseRecord*> _records_spend;

    /// Хранилка расходных покупок.
    std::vector<Purchase*>  _purchases_spend;

    /// Хранилка доходных покупок.
    std::vector<Purchase*>  _purchases_profit;

};

#endif // STORAGE_H
