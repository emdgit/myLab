#ifndef PTRSET_H
#define PTRSET_H

#include <memory>
#include <optional>

#include "Set.h"
#include "templates.h"

template < typename T >
class PtrSet
{

    typedef std::shared_ptr<T>  Ptr;

public:

    PtrSet() {}

    template<typename ...Args>
    void                        append( const Args &...args ) noexcept
    {
        _set.insert( std::make_shared<T>( args... ) );
    }

    size_t                      size() const noexcept
    {
        return _set.size();
    }

    std::optional<Ptr>          find( const T &&val ) const noexcept
    {
        auto it = _set.find( std::forward<T>( std::decay_t<T>( val ) ) );

        auto opt = it == _set.end() ? std::nullopt
                                    : std::make_optional<Ptr>( *it );
        return opt;
    }

    inline decltype ( auto )    begin()  noexcept       { return _set.begin();  }
    inline decltype ( auto )    end()    noexcept       { return _set.end();    }
    inline decltype ( auto )    cbegin() const noexcept { return _set.cbegin(); }
    inline decltype ( auto )    cend()   const noexcept { return _set.cend();   }

    bool operator==( const PtrSet<T> &other ) const noexcept
    {
        if ( size() != other.size() )
            return false;

        auto it1 = _set.cbegin();
        auto it2 = other._set.cbegin();

        for ( ; it1 != _set.cend(); ++it1, ++it2 )
            if ( !( *(*it1).get() == *(*it2).get() ) )
                return false;

        return true;
    }

    bool operator<( const PtrSet<T> &other ) const noexcept
    {
        return size() < other.size();
    }


private:

    Owl::MPSet<T>               _set;

};

#endif // PTRSET_H
