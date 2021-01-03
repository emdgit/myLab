#include "storage.h"

Storage::Storage()
{
    _p_g_storage_spend = new PGStorage();
    _p_g_storage_profit = new PGStorage();
}

PGStorage * Storage::groupsProfit() noexcept
{
    return _p_g_storage_profit;
}

PGStorage * Storage::groupsSpend() noexcept
{
    return _p_g_storage_spend;
}

Storage::record_vec * Storage::recordsProfit() noexcept
{
    return &_records_profit;
}

Storage::record_vec * Storage::recordsSpend() noexcept
{
    return &_records_spend;
}

Storage::purchase_vec * Storage::purchasesProfitSumm() noexcept
{
    return &_purchases_profit_sm;
}

Storage::purchase_vec * Storage::purchasesSpendSumm() noexcept
{
    return &_purchases_spend_sm;
}

Storage::purchase_vec *Storage::purchasesProfit() noexcept
{
    return &_purchases_profit;
}

Storage::purchase_vec *Storage::purchasesSpend() noexcept
{
    return &_purchases_spend;
}

ChartDataStorage * Storage::chartStorage() noexcept
{
    return &_charts_data;
}

