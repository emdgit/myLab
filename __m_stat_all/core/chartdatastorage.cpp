#include "chartdatastorage.h"

ChartDataStorage::ChartDataStorage()
{

}

void ChartDataStorage::setProfits(ChartDataStorage::data_arr && data)
{
    profits_ = std::move(data);
}
