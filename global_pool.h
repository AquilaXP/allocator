#pragma once

#include <type_traits>

#include "small_obj_alloc_thread_safe.h"

class global_pool
{
public:
    using custum_mm = small_obj_allocator_thread_safe;

    static custum_mm& Instance()
    {
        if( init == false )
        {
            ::new( &storage ) custum_mm();
            init = true;
        }
        return reinterpret_cast<custum_mm&>( storage );
    }
private:
    static std::aligned_storage_t<sizeof( custum_mm )> storage;
    static bool init;
};