#include "period.h"

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
