#pragma once

#include <cstddef>
#include <vector>

#include "fixed_allocator.h"

// Сделан на основе описаний в книге Александреско (4 глава)
class small_obj_allocator
{
public:
    small_obj_allocator(size_t chunk_size, size_t max_object_size);
    small_obj_allocator( const small_obj_allocator& ) = delete;
    small_obj_allocator& operator = ( const small_obj_allocator& ) = delete;
    ~small_obj_allocator();

    void* Allocate( size_t num_bytes );
    void Deallocate( void* p, size_t num_bytes );
private:
    size_t m_chunk_size = 0;
    size_t m_max_object_size = 0;
    std::vector<fixed_allocator> m_pool;
    fixed_allocator* m_LastAlloc = nullptr;
    fixed_allocator* m_LastDealloc = nullptr;
};