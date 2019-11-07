#include "field.h"

#ifdef FIELD_DEBUG
#include <QDebug>
#endif

Field::Field(const QString & name) : name(name)
{
#ifdef FIELD_DEBUG
    QString info = "Field( " + name + " )";
    qDebug() << info;
#endif
}

Field::~Field()
{
#ifdef FIELD_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
}
