#include "head.h"



pg::Head::Head()
{

}

void pg::Head::insertField(const pg::FieldPtr &fp, const pg::Head::Type &type) noexcept
{
    _fields.emplace( fp, type );
}

bool pg::Head::operator==(const pg::Head &other) noexcept
{
    return ( _fields == other._fields );
}
