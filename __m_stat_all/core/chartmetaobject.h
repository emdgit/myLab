#pragma once

#include "chartdatastorage.h"

namespace QtCharts {
    class QChart;
}

class ChartMetaObject
{
public:

    using data_arr = ChartDataStorage::data_arr;

    enum Type {
        Unknown,
        Profit,     ///< График всей прибыли
        Spend,      ///< График всех расходов
        Clean,      ///< График чистой прибыли
        ByGroup,    ///< График по конкретной групе
        ByRecord    ///< График по конкретной записи (record)
    };

    ChartMetaObject() = default;
    ChartMetaObject(const Type &t) noexcept;
    ChartMetaObject(const Type &t, QtCharts::QChart *chart) noexcept;
    ChartMetaObject(const Type &t, QtCharts::QChart *chart,
                    data_arr *st) noexcept;

    virtual ~ChartMetaObject() = default;

    virtual void update() const = 0;

    Type type() const noexcept;

    QtCharts::QChart * chart() const noexcept;
    void setChart(QtCharts::QChart *chart) noexcept;

    data_arr * storage() const noexcept;
    void setStorage(data_arr *st) noexcept;


protected:

    Type                type_ = Unknown;

    QtCharts::QChart *  chart_ = nullptr;

    data_arr *          storage_ = nullptr;

};


class ChartMainProfit : public ChartMetaObject
{
public:
    ChartMainProfit() noexcept;
    ChartMainProfit(QtCharts::QChart *chart) noexcept;
    ChartMainProfit(QtCharts::QChart *chart, data_arr *st) noexcept;

    ~ChartMainProfit() override = default;

    // ChartMetaObject interface
    void update() const override;
};
