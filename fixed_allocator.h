#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "chunk.h"

// Выделяем память фиксированного размера
class fixed_allocator
{
    using chuncks = std::vector<chunk>;
public:
    fixed_allocator() = default;
    fixed_allocator( size_t block_size, uint8_t blocks );
    fixed_allocator( const fixed_allocator& ) = delete;
    fixed_allocator& operator = ( const fixed_allocator& ) = delete;
    fixed_allocator(fixed_allocator&& rhs);
    fixed_allocator& operator = ( fixed_allocator&& rhs );
    ~fixed_allocator() = default;

    void Init( size_t block_size, uint8_t blocks );
    void* Allocate();
    void Deallocate( void* p );
    size_t SizeBlock() const;

private:
    size_t m_block_size = 0;
    uint8_t m_num_blocks = 0;    
    chuncks m_chuncks;
    chunk* m_alloc_chucnk = nullptr;
    chunk* m_dealloc_chunck = nullptr;
    //chunk* m_last_empty_chunk = nullptr;
};