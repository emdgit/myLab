#ifndef HEAD_H
#define HEAD_H

#include "typedfield.h"

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


private:

    std::vector<TypedField> _fields;

};

#endif // HEAD_H
