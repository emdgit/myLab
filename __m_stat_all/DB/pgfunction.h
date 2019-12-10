#ifndef PGFUNCTION_H
#define PGFUNCTION_H

#include <QSqlQuery>

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

class PgFunctionPrivate;

class PgFunction
{

    typedef std::vector<FuncArgument>   ArgVec;
    typedef std::vector<FuncInArgument> ArgInVec;

    typedef PgFunctionPrivate           FP;

public:
    PgFunction();
    PgFunction( const QString &name );
    PgFunction( const PgFunction &other );

    inline const QString &  name() const noexcept { return _name; }
    inline const QString &  schema() const noexcept { return _schema; }

    inline void             setSchema( const QString &sch ) noexcept { _schema = sch; }
    inline void             setName( const QString &name ) noexcept { _name = name; }
    void                    addOut(const FuncArgument &&in ) noexcept;
    void                    addIn( const FuncInArgument &&out ) noexcept;

    bool                    isComplete() const noexcept;
    bool                    prepare( QSqlQuery &query ) const noexcept;
    bool                    bindValue( const QString &name,
                                       const QVariant && val) noexcept;

    friend bool operator<( const PgFunction &l, const PgFunction &r ) noexcept;
    friend bool operator==( const PgFunction &l, const PgFunction &r ) noexcept;


private:

    Q_DECLARE_PRIVATE(PgFunction)
    FP *                    d_ptr;

    QString                 _name;
    QString                 _schema;

    ArgVec                  _outArgs;
    ArgInVec                _inArgs;

};

typedef std::shared_ptr<PgFunction> FuncPtr;

#endif // PGFUNCTION_H
