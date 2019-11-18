#include "typestorage.h"
#include "templates.h"

PtrSet<Field> TypeStorage::_fields;
PtrSet<PgFunction> TypeStorage::_funcs;

TypeStorage::TypeStorage() {}

TypeStorage::~TypeStorage() {}

TypeStorage::FieldOpt TypeStorage::field(const QString &name) noexcept
{
    return _fields.find( std::forward<Field>( { name } ) );
}

void TypeStorage::registerField(const QString &name) noexcept
{
    _fields.append( name );
}

void TypeStorage::registerFunc(const PgFunction &func) noexcept
{
    _funcs.append( func );
}
