#pragma once

#include <type_traits>

#include "global_pool.h"

/*
    Аллокатор
    Для выделения памяти используется global_pool.   
*/
template<class T>
class my_allocator
{
public:
    using value_type = T;
    using size_type = size_t;

    using pointer = value_type *;
    using const_pointer = const value_type *;

    using reference = value_type&;
    using const_reference = const value_type&;

    template<class Other>
    struct rebind
    {
        using other = my_allocator<Other>;
    };

    my_allocator() noexcept = default;

    my_allocator( const my_allocator& ) noexcept = default;

    template<class Other>
    my_allocator( const my_allocator<Other>& ) noexcept
    {
    }

    T* allocate( size_type n )
    {
        auto p = global_pool::Instance().Allocate( n * sizeof( T ) );
        return reinterpret_cast<T*>(p);
    }

    void deallocate( T* p, size_type n )
    {
        global_pool::Instance().Deallocate( p, n * sizeof( T ) );
    }

    template< class U, class... Args >
    void construct( U* const p, Args&&... args )
    {
        ::new( p ) U( std::forward<Args>( args )... );
    }

    template< class U >
    void destroy( U* const p )
    {
        p->~U();
    }
};
