#ifndef FIELD_H
#define FIELD_H

#include <QtCore>

#include <memory>

//#define FIELD_DEBUG

struct Field
{
    Field(){}
    Field( const QString &name );
    ~Field();

    QString                     name;

    bool                        operator== ( const Field &other ) noexcept
    {
        return ( name == other.name );
    }

    bool                        operator< ( const Field &other ) noexcept
    {
        return ( name < other.name );
    }
};

typedef std::shared_ptr<Field>  FieldPtr;

#endif // FIELD_H
