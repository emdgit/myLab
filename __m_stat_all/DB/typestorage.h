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

    static FieldPtr                 field( const QString &name,
                                           const QString &type ) noexcept;


private:

    QScopedPointer<TSPrivate>       m_ptr;
    DECLARE_PRIVATE(TypeStorage)

    static FieldSet                 _allFields;

};

#endif // TYPESTORAGE_H
