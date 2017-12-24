#pragma once

#include <memory>
#include <type_traits>

template<class T>
class my_allocator
{
public:
    using value_type = T;

    using pointer = value_type *;
    using const_pointer = const value_type *;

    using reference = value_type&;
    using const_reference = const value_type&;

    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template<class Other>
    struct rebind
    {
        using other = my_allocator<Other>;
    };

    pointer address( reference val ) const noexcept
    {
        return ( std::addressof( val ) );
    }

    const_pointer address( const_reference val ) const noexcept
    {
        return ( std::addressof( val ) );
    }

    my_allocator() noexcept = default;

    my_allocator( const my_allocator& ) noexcept = default;

    template<class Other>
    my_allocator( const my_allocator<Other>& ) noexcept
    {
    }

    T* allocate( std::size_t n )
    {
        auto p = ::malloc( n * sizeof( T ) );
        if( p == nullptr )
            std::bad_alloc();

        return reinterpret_cast<T*>( p );
    }

    void deallocate( T* p, std::size_t n )
    {
        ::free( p );
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

    size_t max_size() const noexcept
    {
        return ( static_cast<size_t>( -1 ) / sizeof( T ) );
    }
};
