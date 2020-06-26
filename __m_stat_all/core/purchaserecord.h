#ifndef PURCHASERECORD_H
#define PURCHASERECORD_H

#include <string>

#include "iobject.h"

/// Ксласс для представления конечной записи
class PurchaseRecord : public IObject
{
public:
    PurchaseRecord();

    // IObject interface
    void                fromPgAnswer(pg::Answer *answer, unsigned long i) override;


    int                 id() const noexcept;
    void                setId(int id) noexcept;

    int                 group_id() const noexcept;
    void                setGroup_id(int group_id) noexcept;

    const std::string & name() const;
    void                setName(const std::string &name);

private:

    int         _id         = 0;
    int         _group_id   = 0;

    std::string _name;

};

#endif // PURCHASERECORD_H
