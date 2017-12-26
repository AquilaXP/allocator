#pragma once

#include <mutex>

#include "small_obj_allocator.h"

// Реализует потоко безосапосное выделение памяти через small_obj_allocator
class small_obj_allocator_thread_safe
{
    using mutex_type = std::mutex;
public:
    small_obj_allocator_thread_safe();
    void* Allocate( size_t size );
    void Deallocate( void* p, size_t size );

private:
    mutex_type m;
    small_obj_allocator soa;
};