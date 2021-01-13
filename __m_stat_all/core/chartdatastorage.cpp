#include "chartdatastorage.h"

ChartDataStorage::ChartDataStorage()
{

}

ChartDataStorage::~ChartDataStorage()
{
    for (const auto &[k, v] : optional_charts) {
        (void)k;
        if (v) {
            delete v;
        }
    }
}

void ChartDataStorage::setProfits(ChartDataStorage::data_arr && data)
{
    profits_ = std::move(data);
}

void ChartDataStorage::setSpends(ChartDataStorage::data_arr && data)
{
    spends_ = std::move(data);
}
