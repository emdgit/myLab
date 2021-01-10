#include "chartmetaobject.h"
#include "coreapi.h"

#include <QtCharts/QChart>

using namespace QtCharts;

ChartMetaObject::ChartMetaObject(const ChartMetaObject::Type &t) noexcept :
    type_(t)
{
}

ChartMetaObject::ChartMetaObject(const ChartMetaObject::Type &t, QChart *chart) noexcept :
    type_(t), chart_(chart)
{
}

ChartMetaObject::ChartMetaObject(const ChartMetaObject::Type &t, QChart *chart,
                                 ChartMetaObject::data_arr *st) noexcept :
    type_(t), chart_(chart), storage_(st)
{
}

ChartMetaObject::Type ChartMetaObject::type() const noexcept
{
    return type_;
}

QChart * ChartMetaObject::chart() const noexcept
{
    return chart_;
}

void ChartMetaObject::setChart(QChart * chart) noexcept
{
    if (chart && chart != chart_) {
        chart_ = chart;
    }
}

ChartMetaObject::data_arr * ChartMetaObject::storage() const noexcept
{
    return storage_;
}

void ChartMetaObject::setStorage(ChartMetaObject::data_arr * st) noexcept
{
    if (st && st != storage_) {
        storage_ = st;
    }
}


ChartMainProfit::ChartMainProfit() noexcept :
    ChartMetaObject(Profit)
{
}

ChartMainProfit::ChartMainProfit(QChart *chart) noexcept :
    ChartMetaObject(Profit, chart)
{
}

ChartMainProfit::ChartMainProfit(QChart *chart, ChartMetaObject::data_arr *st) noexcept :
    ChartMetaObject(Profit, chart, st)
{
}

void ChartMainProfit::update() const
{
//    CoreAPI::loadProfitChartData();
}
