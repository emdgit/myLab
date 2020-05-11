#include "purchasegroup.h"

PurchaseGroup::PurchaseGroup()
{

}

PurchaseGroup::PurchaseGroup(const int &id, const int &pId, const std::string &name, GroupType type) noexcept :
    _id(id), _parentId(pId), _name(name), _type(type)
{

}

const int &PurchaseGroup::id() const noexcept
{
    return _id;
}

const int &PurchaseGroup::parentId() const noexcept
{
    return _parentId;
}

const std::string PurchaseGroup::name() const noexcept
{
    return _name;
}

void PurchaseGroup::setId(int id) noexcept
{
    _id = id;
}

void PurchaseGroup::setParentId(int parentId) noexcept
{
    _parentId = parentId;
}

void PurchaseGroup::setName(const std::string &name) noexcept
{
    _name = name;
}

IObject *PurchaseGroup::fromPgAnswer(pg::Answer *answer, unsigned long i)
{

}
