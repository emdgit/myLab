#ifndef PTRSET_H
#define PTRSET_H

#include <memory>
#include <set>

#include "templates.h"

template < typename T >
class PtrSet
{

    typedef std::shared_ptr<T>  Ptr;

public:

    PtrSet() {}

    void                        append( T * const &val ) noexcept
    {
        _set.insert( std::make_shared( val ) );
    }

    size_t                      size() const noexcept
    {
        return _set.size();
    }

    Ptr                         find( const T &val ) const noexcept
    {
        for ( const auto p : _set )
            if ( *p.data() == val )
                return p;

        return std::make_shared( static_cast<T*>( nullptr ) );
    }

    bool operator==( const PtrSet<T> &other ) const noexcept
    {
        if ( size() != other.size() )
            return false;

        auto it1 = _set.cbegin();
        auto it2 = other._set.cbegin();

        for ( ; it1 != _set.cend(); ++it1, ++it2 )
            if ( *(*it1).data() != *(*it2).data() )
                return false;

        return true;
    }


private:

    std::set<Ptr>               _set;

};

#endif // PTRSET_H
