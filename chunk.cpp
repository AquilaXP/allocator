#include "chunk.h"

#include <utility>
#include <cassert>

chunk::chunk( size_t block_size, uint8_t blocks )
{
    Init( block_size, blocks );
}

chunk& chunk::operator=( chunk&& rhs )
{
    pData = std::move(rhs.pData);
    firstAvailableBlock = rhs.firstAvailableBlock;
    rhs.firstAvailableBlock = 0;
    blocksAvailable = rhs.blocksAvailable;
    rhs.blocksAvailable = 0;

    return ( *this );
}

chunk::chunk( chunk&& rhs )
{
    ( *this ) = std::move( rhs );
}

void chunk::Init( size_t block_size, uint8_t blocks )
{
    pData = std::unique_ptr<uint8_t[]>(new uint8_t[block_size * blocks]);
    firstAvailableBlock = 0;
    blocksAvailable = blocks;
    uint8_t* p = pData.get();
    for( uint8_t i = 0; i != blocks; p += block_size )
    {
        ( *p ) = ++i;
    }
}

void* chunk::Allocate( size_t block_size )
{
    if( blocksAvailable == 0 )
        return nullptr;

    const auto pResult = &pData[ firstAvailableBlock * block_size ];
    firstAvailableBlock = ( *pResult );
    --blocksAvailable;

    return pResult;
}

void chunk::Deallocate( void* p, size_t block_size )
{
    assert( pData.get() <= p );
    uint8_t* toRelease = static_cast<uint8_t*>( p );
    //проверяем выравнивание
    assert( ( ( toRelease - pData.get() ) % block_size ) == 0 );
    ( *toRelease ) = firstAvailableBlock;
    firstAvailableBlock = ( toRelease - pData.get() ) / block_size;
    // проверяем что указатель принадлежит данному чанку
    assert( ( toRelease - pData.get() ) / block_size <= UINT8_MAX );
    ++blocksAvailable;
}

