#ifndef PERIOD_H
#define PERIOD_H

#include <QObject>
#include <QDate>

/// Класс представляет рассчетный период
class Period : public QObject
{
public:

    explicit Period(QObject *parent = nullptr);
    Period(QDate from, QDate to, QObject * parent = nullptr);

    QDate from() const;
    QDate to() const;

    QString toString() const;
    QString fromAsString() const;
    QString toAsString() const;

    void setFrom(QDate from);
    void setTo(QDate to);


private:

    QDate _from;
    QDate _to;

};

#endif // PERIOD_H
