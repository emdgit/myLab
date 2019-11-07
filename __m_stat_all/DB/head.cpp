#include <QCryptographicHash>

#include "head.h"

class HeadPrivate
{
public:

    HeadPrivate( Head * const &_head ) : _head( _head ) {}

    void updateHash() const noexcept
    {
        QString full;

        for ( const auto &f : _head->_fields )
            full += f.get()->name;

        QCryptographicHash hash( QCryptographicHash::Md5 );

        _head->_hash = hash.hash( full.toLatin1(), hash.Md5 );
    }

private:

    Head *      _head;

};

Head::Head()
{
    m_ptr = new HeadPrivate( this );
}

void Head::addField(const QString &name) noexcept
{
    auto size = _fields.size();

    _fields.append( name );

    QQ ( Head );

    if ( _fields.size() != size )
        m->updateHash();
}
