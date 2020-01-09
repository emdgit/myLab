#include "head.h"



pg::Head::Head()
{

}

void pg::Head::insertField(const pg::FieldPtr &fp, const pg::Head::Type &type) noexcept
{
    _fields.emplace_back( fp, type );
}
