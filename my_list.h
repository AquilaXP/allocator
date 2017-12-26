#pragma once
#include <cassert>

template<class MyList>
struct my_iterator
{
    using iterator_category = std::bidirectional_iterator_tag;
    using node_ptr = typename MyList::node*;
    using value_type = typename MyList::value_type;

    my_iterator() = default;
    my_iterator( const my_iterator& rhs ) = default;
    my_iterator( node_ptr ptr )
        : ptr( ptr )
    {

    }

    value_type& operator*()
    {
        return  ptr->value;
    }
    value_type& operator->()
    {
        return  ptr->value;
    }
    my_iterator& operator++()
    {
        ptr = ptr->next;

        return ( *this );
    }

    my_iterator operator++( int )
    {
        my_iterator it( ptr );
        ++( *this );
        return it;
    }

    my_iterator& operator--()
    {
        ptr = ptr->prev;

        return ( *this );
    }

    my_iterator operator--( int )
    {
        my_iterator it( ptr );
        --( *this );
        return it;
    }

    bool operator == ( const my_iterator& rhs ) const
    {
        return ptr == rhs.ptr;
    }

    bool operator != ( const my_iterator& rhs ) const
    {
        return ptr != rhs.ptr;
    }

private:
    node_ptr ptr = nullptr;
};

template<class T, class Alloc = std::allocator<T>>
class my_list
{

public:
    using value_type = T;
    using size_type = size_t;
    using iterator = my_iterator<my_list>;

    struct node
    {
        T value;
        node* next = nullptr; // successor node, or first element if head
        node* prev = nullptr; // predecessor node, or last element if head
    };

    my_list()
    {
        head.next = &head;
        head.prev = &head;
    }
    ~my_list()
    {
        while( empty() == false )
        {
            pop_back();
        }
    }

    void push_back( const T& value )
    {
        node* newNode = m_AllocNode.allocate( 1 );
        try
        {
            m_Alloc.construct( &( newNode->value ), value );
        }
        catch( ... )
        {
            m_Alloc.deallocate( newNode, 1 );
            throw;
        }

        insert( &head, newNode );
    }

    void push_front( const T& value )
    {
        node* newNode = m_AllocNode.allocate( 1 );
        try
        {
            m_Alloc.construct( &( newNode->value ), value );
        }
        catch( ... )
        {
            m_Alloc.deallocate( newNode, 1 );
            throw;
        }

        insert( &head.prev, newNode );
    }

    void pop_front()
    {
        assert( empty() == false );

        node* first = head.next;
        m_Alloc.destroy( &( first->value ) );
        first->next->prev = &head;
        head.next = last->next;
        m_AllocNode.deallocate( last, 1 );
        --m_size;
    }

    void pop_back()
    {
        assert( empty() == false );

        node* last = head.prev;
        m_Alloc.destroy( &(last->value) );
        last->prev->next = &head;
        head.prev = last->prev;
        m_AllocNode.deallocate( last, 1 );
        --m_size;
    }

    void push_back( T&& value )
    {
        node* newNode = m_AllocNode.allocate( 1 );
        try
        {
            m_Alloc.construct( &( newNode->value ), std::forward<T>( value ) );
        }
        catch( ... )
        {
            m_AllocNode.deallocate( newNode, 1 );
            throw;
        }

        insert( &head, newNode );
    }

    bool empty() const
    {
        return size() == 0;
    }

    size_type size() const
    {
        return m_size;
    }

    iterator begin()
    {
        return iterator( head.next );
    }

    iterator end()
    {
        return iterator( &head );
    }

    value_type& front()
    {
        return ( *begin() );
    }

    value_type& back()
    {
        return ( *( --end() ) );
    }

    const value_type& front() const
    {
        return ( *begin() );
    }

    const value_type& back() const
    {
        return ( *( --end() ) );
    }

private:
    void insert( node* where, node* new_node )
    {
        node* prev = where->prev;

        prev->next = new_node;
        where->prev = new_node;

        new_node->prev = prev;
        new_node->next = where;
        ++m_size;
    }

    node head;// = { T{}, &head, &head };
    size_t m_size = 0;
    typename Alloc::template rebind<node>::other m_AllocNode;
    Alloc m_Alloc;
};