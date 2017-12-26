#pragma once

#include <cstdint>
#include <memory>

struct chunk
{
    chunk() = default;
    chunk( size_t block_size, uint8_t blocks );
    chunk( const chunk& ) = delete;
    chunk& operator = ( const chunk& ) = delete;
    chunk(chunk&& rhs);
    chunk& operator = ( chunk&& rhs );
    ~chunk() = default;

    // Нельзя вызывать более 1 раза(конструктор с параметрами так же вызываего Init)
    void Init( size_t block_size, uint8_t blocks );
    void* Allocate( size_t block_size );
    void Deallocate( void* p, size_t block_size );

    std::unique_ptr<uint8_t[]> pData = nullptr;
    uint8_t firstAvailableBlock = 0;
    uint8_t blocksAvailable = 0;
};