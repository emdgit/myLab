#ifndef FIELD_H
#define FIELD_H

#include <QtCore>

#define FIELD_DEBUG

struct Field
{
    Field(){}
    Field( const QString &name,
           const QString &type );
    ~Field();

    QString                     name;
    QString                     type;

    bool                        operator== ( const Field &other ) noexcept
    {
        return ( name == other.name && type == other.type );
    }
};

typedef QSharedPointer<Field>   FieldPtr;

class FieldSet
{
public:
    FieldSet() {}

    void                        push( const FieldPtr &fPtr ) noexcept;

    bool                        contains( const FieldPtr &fPtr ) const noexcept;

    FieldPtr                    find( const QString &name,
                                      const QString &type ) const noexcept;
    Field *                     operator[] ( const int &i )
    {
        return _fSet[i].data();
    }

    bool                        operator==( const FieldSet &other ) const noexcept
    {
        if ( _fSet.size() != other._fSet.size() )
            return false;

        for ( const auto &f : _fSet )
            if ( !other.contains( f ) )
                return false;

        return true;
    }


private:

    QVector<FieldPtr>           _fSet;

};

#endif // FIELD_H
