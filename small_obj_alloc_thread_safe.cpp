#include "small_obj_alloc_thread_safe.h"

small_obj_allocator_thread_safe::small_obj_allocator_thread_safe()
    : soa( 255, 256 )
{
}

void* small_obj_allocator_thread_safe::Allocate( size_t size )
{
    std::lock_guard<mutex_type> g( m );
    return soa.Allocate( size );
}

void small_obj_allocator_thread_safe::Deallocate( void* p, size_t size )
{
    std::lock_guard<mutex_type>g( m );
    return soa.Deallocate( p, size );
}
