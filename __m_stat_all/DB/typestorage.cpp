#include "typestorage.h"
#include "templates.h"

FieldSet TypeStorage::_allFields;



class TypeStoragePrivate
{
public:
    TypeStoragePrivate(){}
};



TypeStorage::TypeStorage() {}

TypeStorage::~TypeStorage() {}

FieldPtr TypeStorage::field(const QString & name, const QString & type) noexcept
{
    using namespace Templates;

    auto ptr = _allFields.find( name, type );

    if ( ptr.isNull() )
    {
        auto field = new Field( name, type );
        auto ptr = makeShared( field );
        _allFields.push( ptr );
        return ptr;
    }

    return ptr;
}
