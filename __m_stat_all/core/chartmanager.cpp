#include "chartmanager.h"
#include "chartmetaobject.h"
#include "coreapi.h"

#include <QDebug>
#include <QQuickItem>
#include <QValueAxis>
#include <QLineSeries>
#include <QGraphicsScene>

#include <QtCharts/QChart>

#include <iostream>

static constexpr auto axis_step = 50000;

class ChartManagerPrivate
{
    using data_arr = ChartManager::data_arr;
public:
    ChartManagerPrivate(ChartManager *cm) :
        cm_(cm) {}

    QtCharts::QChart * findChart(QQuickItem *item) const
    {
        auto scene = item->findChild<QGraphicsScene*>();
        if (!scene) {
            return nullptr;
        }

        for (const auto &ch : scene->items()) {
            auto chart = dynamic_cast<QtCharts::QChart*>(ch);
            if (chart) {
                return chart;
            }
        }

        return nullptr;
    }

    void registerMainChart(QQuickItem *item, ChartMetaObject **target,
                           const ChartMetaObject::Type &type) const
    {
        if (*target) {
            return;
        }

        auto chart = findChart(item);

        if (!chart) {
            throw std::runtime_error("Given wrong parameter. Cannot find a chart");
        }

        ChartManager::data_arr * st = nullptr;

        switch (type) {
            case ChartMetaObject::Profit:
                st = &cm_->storage_->profits_;
                *target = new ChartMainProfit(chart, st);
            break;
            case ChartMetaObject::Spend:
            case ChartMetaObject::Clean:
                throw std::runtime_error("Not Complete");
            break;
            default:
                throw std::runtime_error("Unknown Char Type");
        }
    }

    /// Заполнить данный 'series' значениями из 'arr',
    /// определить максимальное и минимальное значение из набора.
    void fillXYSeries(QtCharts::QXYSeries *series, const data_arr &arr,
                      int &min, int &max) const
    {
        int i(1);

        for (const auto &point : arr) {
            series->append(i, point.first);
            ++i;
            if (point.first < min) {
                min = point.first;
            }
            if (point.first > max) {
                max = point.first;
            }
        }
    }

    ChartMetaObject * findMetaObject(QtCharts::QChart *chart) const
    {
        if (cm_->main_chart_->chart() == chart) {
            return cm_->main_chart_;
        } else if (cm_->clean_profit_chart_->chart() == chart) {
            return cm_->clean_profit_chart_;
        } else {
            for (ChartMetaObject *cmo : cm_->charts_) {
                if (cmo->chart() == chart) {
                    return cmo;
                }
            }
        }

        return nullptr;
    }

private:
    ChartManager *cm_;
};





ChartManager::ChartManager(QObject *parent) :
    QObject(parent), storage_(new ChartDataStorage())
{
    d_ptr = new ChartManagerPrivate(this);
}

ChartManager::~ChartManager()
{
    delete d_ptr;
}

void ChartManager::registerMain(QQuickItem * item)
{
    Q_D(ChartManager);

    d->registerMainChart(item, &main_chart_, ChartMetaObject::Profit);
    updateMain();
}

void ChartManager::registerCleanProfit(QQuickItem * item)
{
    (void)item;
}

void ChartManager::updateMain()
{
    using namespace QtCharts;

    CoreAPI::updateMainChartData();

    int minY(INT_MAX), maxY(-INT_MAX);

    auto chart = main_chart_->chart();
    auto series_profit = new QLineSeries(chart);
    auto series_spend = new QLineSeries(chart);

    Q_D(ChartManager);

    d->fillXYSeries(series_profit, storage_->profits_, minY, maxY);
    d->fillXYSeries(series_spend, storage_->spends_, minY, maxY);

    chart->removeAllSeries();

    series_profit->setName(QString::fromUtf8("Доход"));
    series_profit->setPointsVisible(true);

    series_spend->setName(QString::fromUtf8("Расход"));
    series_spend->setPointsVisible(true);

    if (minY % axis_step != 0) {
        auto min = minY - axis_step;
        minY = (min / axis_step) * axis_step;
    }
    if (maxY % axis_step != 0) {
        auto max = maxY + axis_step;
        maxY = (max / axis_step) * axis_step;
    }

    QValueAxis * yAxies = nullptr;

    auto axes_list = chart->axes(Qt::Vertical);

    if (axes_list.empty()) {
        yAxies = new QValueAxis(chart);
        chart->addAxis(yAxies, Qt::AlignLeft);
    } else {
        yAxies = static_cast<QValueAxis*>(axes_list.front());
    }

    yAxies->setMin(minY);
    yAxies->setMax(maxY);
    yAxies->setTickCount((maxY - minY) / axis_step + 1);

    chart->addSeries(series_profit);
    chart->addSeries(series_spend);

    series_profit->attachAxis(yAxies);
    series_spend->attachAxis(yAxies);

    QPen pen_profit, pen_spend;
    pen_profit.setColor(QColor("#183A8F"));
    pen_profit.setWidthF(2.5);
    pen_spend.setColor(QColor("#FF0700"));
    pen_spend.setWidthF(2.5);

    series_profit->setPen(pen_profit);
    series_spend->setPen(pen_spend);
}

ChartManager::data_arr * ChartManager::chartStorage(QQuickItem * item) const
{
    Q_D(const ChartManager);

    auto chart = d->findChart(item);

    if (!chart) {
        return nullptr;
    }

    auto meta = d->findMetaObject(chart);

    if (!meta) {
        return nullptr;
    }

    return meta->storage();
}

ChartManager::data_arr * ChartManager::storageProfit() const
{
    if (storage_) {
        return &storage_->profits_;
    }

    return nullptr;
}

ChartManager::data_arr * ChartManager::storageSpend() const
{
    if (storage_) {
        return &storage_->spends_;
    }

    return nullptr;
}

ChartManager::data_arr * ChartManager::storageClean() const
{
    if (storage_) {
        return &storage_->clean_profit_;
    }

    return nullptr;
}

