#include "periodmodel.h"
#include "period.h"

PeriodModel::PeriodModel(QObject *parent) : QObject(parent)
{

}

int PeriodModel::size() const
{
    return static_cast<int>(_periods.size());
}

Period *PeriodModel::period(int index) const
{
    return _periods[static_cast<size_t>(index)];
}

QString PeriodModel::periodString(int index) const
{
    return period(index)->toString();
}

QString PeriodModel::dateFrom(int index) const
{
    return period(index)->from().toString("dd.MM.yyyy");
}

QString PeriodModel::dateTo(int index) const
{
    return period(index)->to().toString("dd.MM.yyyy");
}

void PeriodModel::setStartPoint(const QDate &date)
{
    if (date == _startPoint) {
        return;
    }

    _startPoint = date;

    reloadPeriods();
}

void PeriodModel::reloadPeriods()
{
    if (!_startPoint.isValid()) {
        return;
    }

    for (auto &p : _periods) {
        delete p;
    }
    _periods.clear();

    auto st = _startPoint;
    auto fn = _startPoint.addMonths(1).addDays(-1);
    auto cur = QDate::currentDate();

    auto earlier = [&] {
        return st < cur && fn < cur;
    };

    auto current = [&] {
        return st <= cur && cur <= fn;
    };

    while (earlier() || current()) {
        _periods.push_back(new Period(st, fn));
        st = fn.addDays(1);
        fn = fn.addMonths(1);
    }
}
