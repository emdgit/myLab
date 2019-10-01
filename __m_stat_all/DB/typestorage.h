#ifndef TYPESTORAGE_H
#define TYPESTORAGE_H

#include "field.h"
#include "private.h"

class TypeStoragePrivate;
typedef TypeStoragePrivate          TSPrivate;

class TypeStorage
{

public:
    TypeStorage();
    ~TypeStorage();

    static FieldPtr                 field( const QString &name ) noexcept;

    static void                     registerField( const QString &name ) noexcept;


private:

    QScopedPointer<TSPrivate>       m_ptr;
    DECLARE_PRIVATE(TypeStorage)


};

#endif // TYPESTORAGE_H
