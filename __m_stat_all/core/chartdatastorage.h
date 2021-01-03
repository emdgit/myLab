#pragma once

#include <list>
#include <unordered_map>

#include <QString>

/// Хранилище данных для графиков
class ChartDataStorage
{
public:

    using data_arr = std::list<std::pair<double, QString>>;
    using data_map = std::unordered_map<std::string, data_arr*>;

    ChartDataStorage();

    void    setProfits(data_arr &&data);
    void    setSpends(data_arr &&data);


private:

    /// Суммы по доходам за каждый месяц
    data_arr    profits_;
    /// Суммы по расходам за каждый месяц
    data_arr    spends_;
    /// Чистый остаток за каждый месяц
    data_arr    clean_profit_;

    /// Данные для динамически добывляемых графиков
    data_map    optional_charts;
};

