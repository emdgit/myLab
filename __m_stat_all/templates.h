#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <QSharedPointer>

namespace Templates
{
    template< typename Type >
    static QSharedPointer<Type>     makeShared( Type * _data ) noexcept;
}

template< typename Type >
static QSharedPointer<Type> Templates::makeShared( Type * _data ) noexcept
{
    return QSharedPointer< Type >( _data );
}

#endif // TEMPLATES_H
