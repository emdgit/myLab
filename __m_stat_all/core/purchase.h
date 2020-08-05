#ifndef PURCHASE_H
#define PURCHASE_H

#include "iobject.h"

#include <QDate>

#include <string>

/// Класс-обертка над покупкой
class Purchase : public IObject
{
public:
    Purchase();

    void fromPgAnswer(pg::Answer * answer, unsigned long i) override;

    const QDate &   lastDate() const;

    const std::string & name() const;

    double              summ() const noexcept;


private:

    QDate       _last_date;

    std::string _name;

    double      _summ;

};

#endif // PURCHASE_H
