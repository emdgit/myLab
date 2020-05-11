#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <type_traits>

namespace Owl
{
    namespace Templates {
        template < class T, class ... Args >
        struct is_same_m;
    }
}

template < class T, class ... Args >
struct Owl::Templates::is_same_m
{
    static inline constexpr bool inPack = ( std::is_same_v< T, Args > || ... );

    using type = decltype ( inPack ? std::declval<T>
                                   : std::declval< std::false_type > );
};

#endif // TEMPLATES_H
