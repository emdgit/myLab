#include "period.h"

#include <string_view>

static constexpr std::string_view _months[] =
{
    "", "янв", "фев", "мар", "апр", "май", "июн",
    "июн", "авг", "сен", "окт", "ноя", "дек",
};

QString dateToStr(const QDate &date)
{
    auto d = QString::number(date.day());
    auto m = QString(_months[date.month()].data());
    auto y = QString::number(date.year());

    return d + " " + m + " " + y;
}

Period::Period(QObject *parent) : QObject(parent)
{
}

Period::Period(QDate from, QDate to, QObject *parent) :
    QObject (parent), _from(from), _to(to)
{
}

QDate Period::from() const
{
    return _from;
}

QDate Period::to() const
{
    return _to;
}

QString Period::toString() const
{
    auto str = dateToStr(_from) +
               " - " +
               dateToStr(_to);

    return str;
}

void Period::setFrom(QDate from)
{
    if (_from == from)
        return;

    _from = from;
    emit fromChanged(_from);
}

void Period::setTo(QDate to)
{
    if (_to == to)
        return;

    _to = to;
    emit toChanged(_to);
}
