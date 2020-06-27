#include "purchaserecord.h"
#include "templates.h"
#include "answer.h"

using namespace std;

static constexpr size_t _fieldCount = 3;

PurchaseRecord::PurchaseRecord()
{

}

void PurchaseRecord::fromPgAnswer(pg::Answer *answer, unsigned long i)
{
    const auto size = answer->columns();

    if ( size != _fieldCount ) {
        throw runtime_error( "PurchaseGroup::fromPgAnswer: expected 3 fields" );
    }

    QString name;

    fromVariant( _id, answer->field(i,0).value );
    fromVariant( _group_id, answer->field(i,1).value );
    fromVariant( name, answer->field(i,2).value );

    _name = name.toStdString();
}

int PurchaseRecord::id() const noexcept
{
    return _id;
}

void PurchaseRecord::setId(int id) noexcept
{
    _id = id;
}

int PurchaseRecord::group_id() const noexcept
{
    return _group_id;
}

void PurchaseRecord::setGroup_id(int group_id) noexcept
{
    _group_id = group_id;
}

const std::string &PurchaseRecord::name() const
{
    return _name;
}

void PurchaseRecord::setName(const std::string &name)
{
    _name = name;
}
