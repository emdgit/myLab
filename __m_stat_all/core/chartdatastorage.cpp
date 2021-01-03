#include "chartdatastorage.h"

ChartDataStorage::ChartDataStorage()
{

}

void ChartDataStorage::setProfits(ChartDataStorage::data_arr && data)
{
    profits_ = std::move(data);
}

void ChartDataStorage::setSpends(ChartDataStorage::data_arr && data)
{
    spends_ = std::move(data);
}
