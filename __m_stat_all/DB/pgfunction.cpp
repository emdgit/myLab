#include "pgfunction.h"

PgFunction::PgFunction()
{

}

PgFunction::PgFunction(const QString &name) : _name(name) {}

void PgFunction::setIn(const PgFunction::FieldSetP &&in) noexcept
{
    _in = std::move( in );
}

void PgFunction::setOut(const PgFunction::FieldSetP &&out) noexcept
{
    _out = std::move( out );
}

bool PgFunction::bindValue(const QString &name, const QVariant &&val) noexcept
{
    if ( !_in.find( std::make_shared<Field>(name) ) )
        return false;

    _inValues.insert( name, std::move( val ) );

    return true;
}
