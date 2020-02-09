#include "field.h"

pg::Field::Field(const QString & name) noexcept :
    name( name ) {}

pg::Field::~Field() {}

bool pg::Field::operator==(const Field &other) noexcept
{
    return ( name == other.name );
}

bool pg::Field::operator<(const Field &other) noexcept
{
    return ( name < other.name );
}
