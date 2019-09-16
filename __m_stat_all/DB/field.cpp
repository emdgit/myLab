#include "field.h"
#include "templates.h"

#ifdef FIELD_DEBUG
#include <QDebug>
#endif

void FieldSet::push(const FieldPtr & fPtr) noexcept
{
    _fSet.push_back( fPtr );

    std::sort( _fSet.begin(), _fSet.end(), []( const FieldPtr &p1, const FieldPtr &p2 ){
        if ( p1.data()->type == p2.data()->type )
            return p1.data()->name < p2.data()->name;
        return p1.data()->type < p2.data()->type;
    } );
}

bool FieldSet::contains(const FieldPtr & fPtr) const noexcept
{
    auto field = fPtr.data();

    for ( int i(0); i < _fSet.size() && _fSet[i].data()->type <= field->type; ++i )
        if ( *_fSet[i].data() == *field )
            return true;

    return false;
}

FieldPtr FieldSet::find(const QString & name, const QString & type) const noexcept
{
    for ( int i(0); i < _fSet.size() && _fSet[i].data()->type <= type; ++i )
        if ( ( _fSet[i].data()->type == type ) &&
             ( _fSet[i].data()->name == name ) )
            return _fSet[i];

    return Templates::makeShared( static_cast<Field*>( nullptr ) );
}

Field::Field(const QString & name, const QString & type) : name(name), type(type)
{
#ifdef FIELD_DEBUG
    QString info = "Field( " + name + ", " + type + " )";
    qDebug() << info;
#endif
}

Field::~Field()
{
#ifdef FIELD_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}
