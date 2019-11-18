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

    typedef std::vector<FuncArgument>   ArgVec;
    typedef std::vector<FuncInArgument> ArgInVec;

public:
    PgFunction();
    PgFunction( const QString &name );

    inline const QString &  name() const noexcept { return _name; }
    inline const QString &  schema() const noexcept { return _schema; }

    inline void             setSchema( const QString &sch ) noexcept { _schema = sch; }
    inline void             setName( const QString &name ) noexcept { _name = name; }
    void                    addOut(const FuncArgument &&in ) noexcept;
    void                    addIn( const FuncInArgument &&out ) noexcept;

    bool                    bindValue( const QString &name,
                                       const QVariant && val) noexcept;

    friend bool operator<( const PgFunction &l, const PgFunction &r ) noexcept;
    friend bool operator==( const PgFunction &l, const PgFunction &r ) noexcept;


private:

    QString                 _name;
    QString                 _schema;

    ArgVec                  _outArgs;
    ArgInVec                _inArgs;

};

#endif // PGFUNCTION_H
