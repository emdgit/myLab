#ifndef TYPESTORAGE_H
#define TYPESTORAGE_H

#include "head.h"
#include "private.h"

class TypeStoragePrivate;
typedef TypeStoragePrivate          TSPrivate;

class TypeStorage
{

    typedef std::optional<FieldPtr> FieldOpt;
    typedef std::optional<HeadPtr>  HeadOpt;

public:
    TypeStorage();
    ~TypeStorage();

    static FieldOpt                 field( const QString &name ) noexcept;

    static void                     registerField(const QString &name ) noexcept;


private:

    QScopedPointer<TSPrivate>       m_ptr;
    DECLARE_PRIVATE(TypeStorage)

    static PtrSet<Field>            _fields;

    static PtrSet<Head>             _heads;


};

#endif // TYPESTORAGE_H
