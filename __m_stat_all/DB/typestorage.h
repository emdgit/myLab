#ifndef TYPESTORAGE_H
#define TYPESTORAGE_H

#include "private.h"
#include "function.h"

class TypeStorage
{

    typedef std::optional<pg::FieldPtr> FieldOpt;
    typedef std::optional<pg::FuncPtr>  FuncOpt;

public:
    TypeStorage();
    ~TypeStorage();

    static FieldOpt                 field( const QString &name ) noexcept;
    static FuncOpt                  func( const QString &name,
                                          const QString &schema ) noexcept;

    static void                     registerField(const QString &name ) noexcept;
    static void                     registerFunc( const pg::Function &func ) noexcept;


private:

    static PtrSet<pg::Field>        _fields;

    static PtrSet<pg::Function>       _funcs;

};

#endif // TYPESTORAGE_H
