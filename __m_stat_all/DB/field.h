#ifndef FIELD_H
#define FIELD_H

#include <QtCore>

#include <memory>

//#define FIELD_DEBUG

namespace pg {
    struct Field;

    typedef std::shared_ptr<Field>  FieldPtr;
    typedef std::weak_ptr<Field>    FieldPtrW;
}

struct pg::Field
{
    Field();
    Field( const QString &name );
    Field( const Field &field );
    Field( Field && field ) noexcept;
    ~Field();

    QString                     name;

    Field &                     operator=( const Field &other ) noexcept;
    Field &                     operator=( Field &&other ) noexcept;

    bool                        operator== ( const Field &other ) noexcept;
    bool                        operator< ( const Field &other ) noexcept;
};



#endif // FIELD_H
