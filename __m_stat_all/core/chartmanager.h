#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>

class QQuickItem;
class ChartMetaObject;
class ChartManagerPrivate;

#include <memory>

#include "chartdatastorage.h"

/// Класс для управления данными графиков
/*!
 * \brief Здесь регистрируются используемые графики,
 *  вызываются функции обновления.
 */
class ChartManager : public QObject
{

    Q_OBJECT

    using chart_ptr = ChartMetaObject*;
    using chart_data = std::list<chart_ptr>;
    using chart_st = std::unique_ptr<ChartDataStorage>;

public:

    using data_arr = ChartDataStorage::data_arr;

    explicit ChartManager(QObject *parent = nullptr);
    ~ChartManager() override;

    /// Зарегистрировать график общей прибыли
    Q_INVOKABLE
    void registerProfit(QQuickItem *item);

    /// Зарегистрировать график общего расхода
    Q_INVOKABLE
    void registerSpend(QQuickItem *item);

    /// Зарегистрировать график чистой прибыли
    Q_INVOKABLE
    void registerCleanProfit(QQuickItem *item);

    void updateProfit();

    data_arr * chartStorage(QQuickItem *item) const;
    data_arr * storageProfit() const;


private:

    static inline chart_data    charts_;

    chart_ptr   profit_chart_ = nullptr;
    chart_ptr   spend_chart_ = nullptr;
    chart_ptr   clean_profit_chart_ = nullptr;

    chart_st    storage_;

    ChartManagerPrivate * d_ptr;
    Q_DECLARE_PRIVATE(ChartManager);
};

#endif // CHARTMANAGER_H
