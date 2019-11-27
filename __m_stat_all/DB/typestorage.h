#ifndef TYPESTORAGE_H
#define TYPESTORAGE_H

#include "private.h"
#include "pgfunction.h"

class TypeStorage
{

    typedef std::optional<FieldPtr> FieldOpt;
    typedef std::optional<FuncPtr>  FuncOpt;

public:
    TypeStorage();
    ~TypeStorage();

    static FieldOpt                 field( const QString &name ) noexcept;
    static FuncOpt                  func( const QString &name,
                                          const QString &schema ) noexcept;

    static void                     registerField(const QString &name ) noexcept;
    static void                     registerFunc( const PgFunction &func ) noexcept;


private:

    static PtrSet<Field>            _fields;

    static PtrSet<PgFunction>       _funcs;

};

#endif // TYPESTORAGE_H
