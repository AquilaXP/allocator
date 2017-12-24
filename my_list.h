#pragma once

template<class MyList>
struct my_iterator
{
    using node_ptr = typename MyList::node*;
    using value_type = typename MyList::value_type;

    node_ptr ptr = nullptr;

    my_iterator() = default;
    my_iterator( const my_iterator& rhs ) = default;
    my_iterator( node_ptr ptr ) : ptr( ptr )
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
        ptr = ptr->prev;
        return it;
    }
    bool operator != ( const my_iterator& rhs ) const
    {
        return rhs.ptr != ptr;
    }
};

template<class T, class Alloc = std::allocator<T>>
class my_list
{

public:
    using value_type = T;
    using iterator = my_iterator<my_list>;

    my_list() = default;
    my_list( const Alloc& alloc )
        : m_Alloc(alloc), m_AllocNode(alloc)
    {        
    }

    struct node
    {
        T value;
        node* next = nullptr; // successor node, or first element if head
        node* prev = nullptr; // predecessor node, or last element if head
    };

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

    iterator begin()
    {
        return iterator( head.next );
    }
    iterator end()
    {
        return iterator( &head );
    }
    void insert( node* where, node* new_node )
    {
        node* prev = where->prev;

        prev->next = new_node;
        where->prev = new_node;

        new_node->prev = prev;
        new_node->next = where;
        ++size;
    }
private:

    node head = { T{} ,&head, &head };
    size_t size = 0;
    typename Alloc::template rebind<node>::other m_AllocNode;
    Alloc m_Alloc;
};