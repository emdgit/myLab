#ifndef STORAGE_H
#define STORAGE_H

#include <vector>

#include "storagedefinition.h"
#include "chartdatastorage.h"
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

    /// Вернуть указатель на хранилище СУММ доходных транзакций
    purchase_vec *   purchasesProfitSumm() noexcept;
    /// Вернуть указатель на хранилище СУММ расходных транзакций
    purchase_vec *   purchasesSpendSumm() noexcept;

    /// Вернуть указатель на хранилище доходных транзакций
    purchase_vec *   purchasesProfit() noexcept;
    /// Вернуть указатель на хранилище расходных транзакций
    purchase_vec *   purchasesSpend() noexcept;

    ChartDataStorage * chartStorage() noexcept;


private:

    /// Хранилка групп доходов.
    PGStorage * _p_g_storage_profit;

    /// Хранилка групп попупок.
    PGStorage * _p_g_storage_spend;

    /// Хранилка доходных записей.
    std::vector<PurchaseRecord*> _records_profit;

    /// Хранилка расходных записей.
    std::vector<PurchaseRecord*> _records_spend;

    /// Хранилка расходных покупок. Хранит сумму покупок по
    /// конкретной группе за период
    std::vector<Purchase*>  _purchases_spend_sm;

    /// Хранилка доходных покупок. (ЗП) Хранит сумму ЗП по
    /// конкретной группе за период
    std::vector<Purchase*>  _purchases_profit_sm;

    /// Хранилка расходных покупок. Хранит покупки по
    /// конкретной группе за период
    std::vector<Purchase*>  _purchases_spend;

    /// Хранилка доходных покупок. (ЗП) Хранит ЗП по
    /// конкретной группе за период
    std::vector<Purchase*>  _purchases_profit;

    /// Данные графиков
    ChartDataStorage        _charts_data;
};

#endif // STORAGE_H
