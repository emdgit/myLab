#include "purchasegroup.h"
#include "templates.h"
#include "answer.h"

using namespace std;

static constexpr size_t _fieldCount = 3;

static constexpr const char* _fieldNames[3] = {
    "id",
    "group_name",
    "group_parent_id"
};

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

void PurchaseGroup::fromPgAnswer(pg::Answer *answer, unsigned long i)
{
    const auto size = answer->size();

    if ( size != _fieldCount ) {
        throw runtime_error( "PurchaseGroup::fromPgAnswer: expected 3 fields" );
    }

    QString name;

    fromVariant( _id, answer->field(i,0).value );
    fromVariant( name, answer->field(i,1).value );
    fromVariant( _parentId, answer->field(i,2).value );

    _name = name.toStdString();
}
