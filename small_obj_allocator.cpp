#include "small_obj_allocator.h"

#include <cassert>
#include <iostream>

small_obj_allocator::small_obj_allocator( size_t chunk_size, size_t max_object_size ) : m_chunk_size( chunk_size ), m_max_object_size( max_object_size )
{
    assert( chunk_size <= UINT8_MAX );
}

small_obj_allocator::~small_obj_allocator()
{
}

void* small_obj_allocator::Allocate( size_t num_bytes )
{
    if( num_bytes > m_max_object_size )
        return ::operator new (num_bytes);

    if( m_LastAlloc == nullptr || m_LastAlloc->SizeBlock() != num_bytes )
    {
        m_LastAlloc = nullptr;
        for( auto& fa : m_pool )
        {
            if( num_bytes == fa.SizeBlock() )
            {
                m_LastAlloc = &fa;
            }
        }

        if( m_LastAlloc == nullptr )
        {
            m_pool.emplace_back( num_bytes, m_chunk_size );
            m_LastAlloc = &( m_pool.back() );
        }
    }

    return m_LastAlloc->Allocate();
}

void small_obj_allocator::Deallocate( void* p, size_t num_bytes )
{
    if( num_bytes > m_max_object_size )
    {
        ::operator delete( p );
        return;
    }

    if( m_LastDealloc == nullptr || m_LastDealloc->SizeBlock() != num_bytes )
    {
        for( auto& fa : m_pool )
        {
            if( num_bytes == fa.SizeBlock() )
            {
                m_LastDealloc = &fa;
            }
        }
    }

    assert( m_LastDealloc != nullptr );
    assert( m_LastDealloc->SizeBlock() == num_bytes );

    m_LastDealloc->Deallocate( p );
}

