#include "typedfield.h"

pg::TypedField::TypedField() :
    type(Type::UnknownType)
{
}

pg::TypedField::TypedField(const FieldPtr &field, const TypedField::Type &t) :
    field( field ), type( t ) {}

pg::TypedField::TypedField(const TypedField &tf) :
    field(tf.field), type(tf.type)
{
}

bool pg::TypedField::operator==(const TypedField &other) const noexcept
{
    return ( field.lock().get() == other.field.lock().get() &&
             type == other.type );
}

bool pg::TypedField::operator<(const TypedField &other) const noexcept
{
    if ( type < other.type )
        return true;

    return field.lock().get() < other.field.lock().get();
}

