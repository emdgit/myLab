#ifndef PGFUNCTION_H
#define PGFUNCTION_H

#include <QString>
#include <QVariantMap>

#include "field.h"
#include "ptrset.h"

struct FuncArgument
{
    FieldPtr    field;
};

struct FuncInArgument : FuncArgument
{
    bool        isDefault = false;

    QVariant    value;
};

class PgFunction
{

    using FieldSetP = PtrSet<Field>;

public:
    PgFunction();
    PgFunction( const QString &name );

    inline const QString &  name() const noexcept { return _name; }

    void                    setIn( const FieldSetP &&in ) noexcept;
    void                    setOut( const FieldSetP &&out ) noexcept;

    bool                    bindValue( const QString &name,
                                       const QVariant && val) noexcept;


private:

    QString                 _name;

    FieldSetP               _in;
    FieldSetP               _out;

    QVariantMap             _inValues;
};

#endif // PGFUNCTION_H
