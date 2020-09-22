#include "period.h"

#include <string_view>

static constexpr std::string_view _months[] =
{
    // Первый пуст, потому что индекстация в
    // QDate с единицы.
    "", "янв", "фев", "мар", "апр", "май", "июн",
    "июл", "авг", "сен", "окт", "ноя", "дек",
};

QString dateToStr(const QDate &date)
{
    if (!date.isValid()) {
        return {};
    }

    auto d = QString::number(date.day());
    auto m = QString(_months[date.month()].data());
    auto y = QString::number(date.year());

    return d + " " + m + " " + y;
}

Period::Period(QObject *parent) : QObject(parent)
{
}

Period::Period(std::pair<QDate, QDate> date_pair, QObject *parent) :
    QObject (parent), _from(std::min(date_pair.first, date_pair.second)),
    _to(std::max(date_pair.first, date_pair.second))
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

QString Period::fromAsString() const
{
    return dateToStr(_from);
}

QString Period::toAsString() const
{
    return dateToStr(_to);
}

void Period::setFrom(QDate from)
{
    if (_from == from)
        return;

    _from = from;
}

void Period::setTo(QDate to)
{
    if (_to == to)
        return;

    _to = to;
}

bool Period::containsDate(const QDate & date) const
{
    return (_from <= date && date <= _to);
}

bool Period::operator==(const Period &other) const
{
    return (_from == other.from() && _to == other.to());
}
