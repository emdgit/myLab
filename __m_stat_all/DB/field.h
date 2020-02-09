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
    Field() = default;
    Field( const Field &field ) = default;
    Field( Field && field ) = default;
    Field( const QString &name ) noexcept;
    ~Field();

    QString                     name;

    Field &                     operator=( const Field &other ) = default;
    Field &                     operator=( Field &&other ) = default;

    bool                        operator== ( const Field &other ) noexcept;
    bool                        operator< ( const Field &other ) noexcept;
};



#endif // FIELD_H
