#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

namespace Owl
{
    namespace Exception
    {
        template< typename T >
        class Exception;
    }
}

template< typename T >
class Owl::Exception::Exception
{

public:

    Exception( const std::string &what ) : _what( what ) {}

    Exception() = default;
    Exception( Exception &&other ) = default;

    Exception( const Exception &other ) = delete;
    Exception &operator=(const Exception &other) = delete;

    virtual const char * what() const noexcept { return _what.data(); }

    inline std::string & whatStr() noexcept { return _what; }

private:

    std::string    _what;

};

template< typename T >
static inline constexpr auto makeException( T _t, const std::string &what ) noexcept
{
    using type = std::remove_cv_t< std::remove_pointer_t< decltype (_t) > >;

    return Owl::Exception::Exception<type>( what );
}

template< typename T >
static inline constexpr auto makeException( const std::string &what ) noexcept
{
    using type = std::remove_cv_t< std::remove_pointer_t< T > >;

    return Owl::Exception::Exception<type>( what );
}

#endif // EXCEPTION_H
