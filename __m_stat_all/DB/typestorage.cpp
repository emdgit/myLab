#include "typestorage.h"
#include "templates.h"

PtrSet<Field> TypeStorage::_fields;

PtrSet<Head> TypeStorage::_heads;



class TypeStoragePrivate
{
public:
    TypeStoragePrivate(){}
};



TypeStorage::TypeStorage() {}

TypeStorage::~TypeStorage() {}

TypeStorage::FieldOpt TypeStorage::field(const QString &name) noexcept
{
    return _fields.find( { name } );
}

void TypeStorage::registerField(const QString &name) noexcept
{
    _fields.append( name );
}
