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

class ChartManagerPrivate
{
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

    ChartMetaObject * findMetaObject(QtCharts::QChart *chart) const
    {
        if (cm_->profit_chart_->chart() == chart) {
            return cm_->profit_chart_;
        } else if (cm_->spend_chart_->chart() == chart) {
            return cm_->spend_chart_;
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

void ChartManager::registerProfit(QQuickItem * item)
{
    Q_D(ChartManager);

    d->registerMainChart(item, &profit_chart_, ChartMetaObject::Profit);
    CoreAPI::updateProfitChartData();
}

void ChartManager::registerSpend(QQuickItem * item)
{
    (void)item;
}

void ChartManager::registerCleanProfit(QQuickItem * item)
{
    (void)item;
}

void ChartManager::updateProfit()
{
    using namespace QtCharts;

    int i(1), minY(INT_MAX), maxY(-INT_MAX);

    auto chart = profit_chart_->chart();
    auto series = new QLineSeries(chart);

    for (const auto &point : *profit_chart_->storage()) {
        series->append(i, point.first);
        ++i;
        if (point.first < minY) {
            minY = point.first;
        }
        if (point.first > maxY) {
            maxY = point.first;
        }
    }

    chart->removeAllSeries();

    series->setName(QString::fromUtf8("Доход"));
    series->setPointsVisible(true);

    minY -= 0.05 * (maxY - minY);
    maxY += 0.05 * (maxY - minY);

    auto yAxies = new QValueAxis(chart);
    yAxies->setMin(minY);
    yAxies->setMax(maxY);

    chart->addAxis(yAxies, Qt::AlignLeft);
    chart->addSeries(series);
    series->attachAxis(yAxies);

    QPen pen;
    pen.setColor(QColor("#183A8F"));
    pen.setWidthF(2.5);
    series->setPen(pen);
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
    if (profit_chart_) {
        return profit_chart_->storage();
    }

    return nullptr;
}

