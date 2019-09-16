#ifndef ENTITY_H
#define ENTITY_H

#include "head.h"

class Entity
{
public:
    Entity();
    Entity( const QString &name );
    Entity( const QString &name,
            const HeadPtr &ptr );

    inline void             setName( const QString &name ) noexcept { _name = name; }
    inline void             setHead( const HeadPtr &ptr ) noexcept { _head = ptr; }

    inline const QString &  name() const noexcept { return _name; }
    inline const HeadPtr &  head() const noexcept { return _head; }


private:

    HeadPtr                 _head;

    QString                 _name;


};

#endif // ENTITY_H
