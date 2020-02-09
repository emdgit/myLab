#ifndef HEAD_H
#define HEAD_H

#include "typedfield.h"
#include "ptrset.h"

namespace pg {
    class Head;
}

class pg::Head
{

    using Type = QMetaType::Type;
    using FSet = std::set< TypedField >;

public:

    Head();

    void    insertField( const pg::FieldPtr &fp,
                         const Type &type ) noexcept;

    bool    operator==( const pg::Head &other ) noexcept;


private:

    FSet    _fields;

};

#endif // HEAD_H
