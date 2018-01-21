#include "fixed_allocator.h"
#include <cassert>

fixed_allocator::fixed_allocator( size_t block_size, uint8_t blocks )
{
    Init( block_size, blocks );
}

fixed_allocator::fixed_allocator( fixed_allocator&& rhs ) 
    : m_block_size( rhs.m_block_size ),
    m_num_blocks( rhs.m_num_blocks ),
    m_chuncks( std::move( rhs.m_chuncks ) ),
    m_alloc_chucnk( rhs.m_alloc_chucnk ),
    m_dealloc_chunck( rhs.m_dealloc_chunck )//,
    //m_last_empty_chunk( rhs.m_last_empty_chunk )
{
    rhs.m_alloc_chucnk = nullptr;
    rhs.m_dealloc_chunck = nullptr;
    //rhs.m_last_empty_chunk = nullptr;
    rhs.m_block_size = 0;
    rhs.m_num_blocks = 0;
}

fixed_allocator& fixed_allocator::operator=( fixed_allocator&& rhs )
{
    m_block_size = rhs.m_block_size;
    rhs.m_block_size = 0;
    m_num_blocks = rhs.m_num_blocks;
    rhs.m_num_blocks = 0;
    m_chuncks = std::move( rhs.m_chuncks );
    m_alloc_chucnk = rhs.m_alloc_chucnk;
    rhs.m_alloc_chucnk = nullptr;
    m_dealloc_chunck = rhs.m_dealloc_chunck;
    rhs.m_dealloc_chunck = nullptr;
    //m_last_empty_chunk = rhs.m_last_empty_chunk;
    //rhs.m_last_empty_chunk = nullptr;

    return ( *this );
}

void fixed_allocator::Init( size_t block_size, uint8_t blocks )
{
    assert( m_chuncks.empty() );

    m_block_size = block_size;
    m_num_blocks = blocks;
}

void* fixed_allocator::Allocate()
{
    if( m_alloc_chucnk == nullptr || m_alloc_chucnk->blocksAvailable == 0 )
    {
        m_alloc_chucnk = nullptr;
        for( auto& c : m_chuncks )
        {
            if( c.blocksAvailable != 0 )
                m_alloc_chucnk = &c;
        }

        if( m_alloc_chucnk == nullptr )
        {
            m_chuncks.emplace_back( m_block_size, m_num_blocks );
            m_alloc_chucnk = &( m_chuncks.back() );
        }
    }

    assert( m_alloc_chucnk != nullptr );
    assert( m_alloc_chucnk->blocksAvailable != 0 );

    auto p = m_alloc_chucnk->Allocate( m_block_size );
    // воспользовались последним пустым чанком, теперь нету пустых
/*    if( m_alloc_chucnk->blocksAvailable == ( m_num_blocks - 1 ) &&
        m_last_empty_chunk == m_alloc_chucnk )
    {
        m_last_empty_chunk = nullptr;
    }
*/
    return p;
}

void fixed_allocator::Deallocate( void* p )
{
    if( m_dealloc_chunck == nullptr || m_dealloc_chunck->blocksAvailable == m_num_blocks )
    {
        for( auto& c : m_chuncks )
        {
            if( c.pData.get() <= p && p <= ( c.pData.get() + m_num_blocks * m_block_size ) )
            {
                m_dealloc_chunck = &c;
                break;
            }
        }
    }

    assert( m_dealloc_chunck != nullptr );
    assert( m_dealloc_chunck->blocksAvailable != m_num_blocks );

    m_dealloc_chunck->Deallocate( p, m_block_size );

    // чанк пустой?
    if( m_dealloc_chunck->blocksAvailable == m_num_blocks )
    {
        // Если нету пустого чанка, сохраняем
/*        if( m_last_empty_chunk == nullptr )
        {
            m_last_empty_chunk = m_dealloc_chunck;
            return;
        }
        else*/
        {
            auto c = m_chuncks.begin();
            for( ; c != m_chuncks.end(); ++c )
            {
                if( m_dealloc_chunck == &( *c ) )
                {
                    break;
                }
            }
            m_chuncks.erase( c );
        }
    }
}

size_t fixed_allocator::SizeBlock() const
{
    return m_block_size;
}

