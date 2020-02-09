#ifndef PGFUNCTION_H
#define PGFUNCTION_H

#include <QSqlQuery>

#include <QString>
#include <QVariantMap>

#include "field.h"
#include "ptrset.h"

struct FuncArgument
{
    pg::FieldPtr  field;
};

struct FuncInArgument : FuncArgument
{
    bool        isDefault = false;

    QVariant    value;
};

class PgFunctionPrivate;

namespace pg {
    class Function;

    typedef std::shared_ptr<Function> FuncPtr;
}

class pg::Function
{

    using ArgVec = std::vector<FuncArgument>;
    using ArgInVec = std::vector<FuncInArgument>;

    using FP = PgFunctionPrivate;

public:
    Function();
    Function( const QString &name );
    Function( const Function &other );

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

    bool                    operator<( const pg::Function &other ) noexcept;
    bool                    operator==( const pg::Function &other ) noexcept;


private:

    Q_DECLARE_PRIVATE(PgFunction)
    friend class ::PgFunctionPrivate;
    FP *                    d_ptr;

    QString                 _name;
    QString                 _schema;

    ArgVec                  _outArgs;
    ArgInVec                _inArgs;

};

#endif // PGFUNCTION_H
