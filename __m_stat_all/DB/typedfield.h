#ifndef TYPEDFIELD_H
#define TYPEDFIELD_H

#include "field.h"

namespace pg {
    struct TypedField;
}

struct pg::TypedField
{
    using Type = QMetaType::Type;

    TypedField();
    TypedField( const FieldPtr &field, const Type &t );
    TypedField( const TypedField &tf );

    FieldPtrW   field;

    Type        type;

    bool        operator== ( const TypedField &other ) noexcept;
    bool        operator< ( const TypedField &other ) noexcept;
};

#endif // TYPEDFIELD_H
