#ifndef HEAD_H
#define HEAD_H

#include "field.h"
#include "ptrset.h"
#include "private.h"

class HeadPrivate;

class Head
{
public:

    Head();

    inline const QString &  hash() const noexcept   { return _hash; }

    void                    addField( const QString &name ) noexcept;

    bool operator==( const Head &other ) const noexcept
    {
        return _hash == other._hash;
    }

    bool operator<( const Head &other ) const noexcept
    {
        return _fields < other._fields;
    }


private:

    DECLARE_PRIVATE(Head)
    HeadPrivate *           m_ptr;

    PtrSet< Field >         _fields;

    QString                 _hash;


};

typedef std::shared_ptr<Head>   HeadPtr;

#endif // HEAD_H
