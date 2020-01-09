#include "field.h"

#ifdef FIELD_DEBUG
#include <QDebug>
#endif

pg::Field::Field(){}

pg::Field::Field(const QString & name) : name(name)
{
#ifdef FIELD_DEBUG
    QString info = "Field( " + name + " )";
    qDebug() << info;
#endif
}

pg::Field::Field(const Field &field)
{
    name = field.name;
}

pg::Field::Field(Field &&field) noexcept : name( std::move( field.name ) ) {}

pg::Field::~Field()
{
#ifdef FIELD_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}

pg::Field &pg::Field::operator=(const Field &other) noexcept
{
    name = other.name;

    return *this;
}

pg::Field &pg::Field::operator=(Field &&other) noexcept
{
    name = std::move( other.name );

    return *this;
}

bool pg::Field::operator==(const Field &other) noexcept
{
    return ( name == other.name );
}

bool pg::Field::operator<(const Field &other) noexcept
{
    return ( name < other.name );
}
