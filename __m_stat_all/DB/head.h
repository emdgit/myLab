#ifndef HEAD_H
#define HEAD_H

#include "typedfield.h"
#include "Set.h"

namespace pg {
    class Head;
}

class pg::Head
{

    using Type = QMetaType::Type;

public:

    Head();

    void                    insertField( const pg::FieldPtr &fp,
                                         const Type &type ) noexcept;

    bool                    operator== ( const pg::Head &other ) noexcept;


private:

    Owl::MSet<TypedField>   _fields;

};

#endif // HEAD_H
