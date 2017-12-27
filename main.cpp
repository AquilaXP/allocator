#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>

#include "my_allocator.h"
#include "my_list.h"

using namespace std;

uint32_t factorial( uint32_t num )
{
    if( num == 0 )
        return 1;
    return num * factorial( num - 1 );
}

template<class K, class T, class P, class A>
void insert_factorial_to_map( std::map<K,T,P,A>& m, uint32_t count )
{
    for( uint32_t i = 0; i < count; ++i )
    {
        m.insert( std::make_pair( i, factorial( i ) ) );
    }
}

template<class K, class T, class P, class A>
decltype(auto) operator << ( std::ostream& ostr, std::map<K,T,P,A>& m )
{
    for( auto& p : m )
    {
        ostr << p.first << ' ' << p.second << '\n';
    }
    return ostr;
}

template<class T, class A>
decltype( auto ) operator << ( std::ostream& ostr, my_list<T,A>& l )
{
    for( auto& v : l )
    {
        ostr << v << '\n';
    }
    return ostr;
}

int main()
{
    {
        /// Стандартный map стандртный allocator
        std::map<uint32_t, uint32_t> test_map;
        insert_factorial_to_map( test_map, 10 );
        std::cout << "Default allocator map\n";
        std::cout << test_map;
    }
    {
        /// Стандартный map мой allocator
        std::map<uint32_t,
            uint32_t,
            std::less<uint32_t>,
            my_allocator<std::pair<const uint32_t, uint32_t>>> test_map_my_alloc;
        insert_factorial_to_map( test_map_my_alloc, 10 );
        std::cout << "My allocator map\n";
        std::cout << test_map_my_alloc;
    }
    {
        /// Мой простой stl совместимый двух связный список и стандартный allocator
        my_list<int> test_my_list;
        int i = 0;
        std::generate_n( std::back_inserter( test_my_list ), 10,
            [&i](){ return i++; } );
        std::cout << "My list\n";
        std::cout << test_my_list;
    }
    {
        /// Мой простой stl совместимый двух связный список и мой allocator
        my_list<int, my_allocator<int>> test_my_list_my_alloc;
        int i = 0;
        std::generate_n( std::back_inserter( test_my_list_my_alloc ), 10,
            [&i](){ return i++; } );
        std::cout << "My list my allocator\n";
        std::cout << test_my_list_my_alloc;
    }
    return 0;
}