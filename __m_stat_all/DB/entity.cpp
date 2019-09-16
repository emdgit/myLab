#include "entity.h"

Entity::Entity()
{
    _head = QSharedPointer<Head>( static_cast<Head*>( nullptr ) );
}

Entity::Entity(const QString & name) : _name(name) {}

Entity::Entity(const QString & name, const HeadPtr & ptr) :
    _head(ptr), _name(name) {}
