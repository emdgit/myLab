#ifndef HEAD_H
#define HEAD_H

#include "field.h"

struct Head
{
    FieldSet                _fields;

    bool                    operator==( const Head &other ) noexcept
    {
        return _fields == other._fields;
    }
};

typedef QSharedPointer<Head>    HeadPtr;

#endif // HEAD_H
