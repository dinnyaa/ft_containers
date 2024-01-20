#pragma once

#include <iostream>
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "bidirectional_iterator.hpp"
#include "algorithm.hpp"
#include <new>
#include "RBT.hpp"

namespace ft
{
    template<class Key, class Compare = std::less<Key>,
        class Allocator = std::allocator<Key> >
    class set
    {
    public:
        typedef Key                                             key_type;
        typedef Key                                             value_type;
        typedef std::size_t                                     size_type;
        typedef std::ptrdiff_t                                  difference_type;
        typedef Compare                                         key_compare;
        typedef Compare                                         value_compare;
        typedef Allocator                                       allocator_type;
        typedef value_type&                                     reference;
        typedef const value_type&                               const_reference;
        typedef typename Allocator::pointer                     pointer;
        typedef typename Allocator::const_pointer               const_pointer;
        typedef ft::bidirectional_const_iterator<value_type>    iterator;
        typedef ft::bidirectional_const_iterator<value_type>    const_iterator;
        typedef ft::reverse_iterator<const_iterator>            reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>            const_reverse_iterator;

        typedef  Node<value_type>*                              NodePtr;

    private:

                
        template <typename T>
        struct Identity
        {
            typedef T key_type;
            T &operator()(T &val)
            {
                return val;
            }

            const T &operator()(T &val) const
            {
                return val;
            }
        };

        RBT<value_type, Identity<value_type>, Compare>   _tree;
        allocator_type                                   _alloc;

    public:
        /*              Constructors            */

        set() : _tree(Compare()) {}
        explicit set( const Compare& comp, 
            const Allocator& alloc = Allocator() ) : _tree(comp), _alloc(alloc) {}

        template< class InputIt >
        set( InputIt first, InputIt last, const Compare& comp = Compare(),
            const Allocator& alloc = Allocator(),
            typename ft::enable_if <!ft::is_integral<InputIt>::value, bool>::type = 0)
            : _tree(comp), _alloc(alloc)
        {
            while (first != last)
            {
                insert(*first);
                ++first;
            }
        }

        set( const set& other )
        : _tree(other._tree) {}

        /*              Destructor             */

        ~set()
        {
            _tree.delete_all();
            _tree.delete_NIL();
        }

        set& operator=( const set& other )
        {
            _tree = other._tree;
            return *this;
        }

        allocator_type get_allocator() const
        {
            return _alloc;
        }

        /*              Iterators               */

        iterator begin()
        {
            if (_tree.size() == 0)
                return iterator(_tree.getNil());
            else
                return iterator(_tree.minimum());
        }

        const_iterator begin() const
        {
            if (_tree.size() == 0)
                return const_iterator(_tree.getNil());
            else
                return const_iterator(_tree.minimum());
        }

        iterator end()
        {
            iterator tmp (_tree.getNil());
            return tmp;
        }

        const_iterator end() const
        {
            return const_iterator(_tree.getNil());
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        /*              Capacity            */
        bool empty() const
        {
            return _tree.size() == 0;
        }

        size_type size() const
        {
            return _tree.size();
        }

        size_type max_size() const
         {
            return _alloc.max_size();
        }

        /*              Modifiers           */
        void clear()
        {
            _tree.delete_all();
        }

        ft::pair<iterator, bool> insert( const value_type& value )
        {
            NodePtr tmp = _tree.search(value);
            ft::pair<iterator, bool> ret;
            if (tmp == _tree.getNil())
            {
                ret.first = iterator(_tree.insert(value));
                ret.second = true;
            }
            else
            {
                ret.first = iterator(tmp);
                ret.second = false;
            }
            return ret;
        }

        iterator insert( iterator pos, const value_type& value )
        {
            NodePtr tmp = _tree.search(value);
            if (tmp == _tree.getNil())
                return iterator(_tree.insert(pos, value));
            else
                return iterator(tmp);
        }


        template< class InputIt >
        void insert( InputIt first, InputIt last, typename ft::enable_if <!ft::is_integral<InputIt>::value, bool>::type = 0)
         {
            while (first != last)
            {
                if(_tree.search(*first) == _tree.getNil())
                    _tree.insert(*first);
                ++first;
            }
        }

        void erase( iterator pos )
         {
            _tree.remove(*pos);
        }

        void erase( iterator first, iterator last )
        {
            NodePtr tmp = first.base();
            value_type last_pair = *last;
            while (tmp->key != last_pair)
            {
               _tree.remove(tmp->key);
                tmp = _tree.upper_bound(tmp->key);
            }
        }

        size_type erase( const Key& key )
        {
            if (_tree.search(key) != _tree.getNil())
            {
                _tree.remove(key);
                return 1;
            }
            return 0;
        }

        void swap( set& other )
        {
            this->_tree.swap(other._tree);
        }

        size_type count( const Key& key ) const
        {
            return (_tree.search(key) == _tree.getNil() ? 0 : 1);
        }

        iterator find( const Key& key )
        {
            return iterator(_tree.search(key));
        }

        const_iterator find( const Key& key ) const
        {
            return const_iterator(_tree.search(key));
        }

            ft::pair<iterator,iterator> equal_range( const Key& key )
            {
                return ft::make_pair(lower_bound(key), upper_bound(key));
            }

            ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
            {
                return ft::make_pair(lower_bound(key), upper_bound(key));
            }
            iterator lower_bound( const Key& key )
            {
                return iterator(_tree.lower_bound(key));
            }

            const_iterator lower_bound( const Key& key ) const
            {
                return const_iterator(_tree.lower_bound(key));
            }

            iterator upper_bound( const Key& key )
            {
                return iterator(_tree.upper_bound(key));
            }

            const_iterator upper_bound( const Key& key ) const
            {
                return const_iterator(_tree.upper_bound(key));
            }

            key_compare key_comp() const
            {
                return key_compare();
            }

            value_compare value_comp() const
            {
                return (value_compare());
            }
    };

    template< class Key, class Compare, class Alloc >
    bool operator==( const ft::set<Key,Compare,Alloc>& lhs,
                 const ft::set<Key,Compare,Alloc>& rhs )
    {
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class Key, class Compare, class Alloc >
    bool operator!=( const ft::set<Key,Compare,Alloc>& lhs,
                 const ft::set<Key,Compare,Alloc>& rhs )
    {
        return !(lhs == rhs);
    }

    template< class Key, class Compare, class Alloc >
    bool operator<( const ft::set<Key,Compare,Alloc>& lhs,
                const ft::set<Key,Compare,Alloc>& rhs )
    {
         return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class Key, class Compare, class Alloc >
    bool operator>( const ft::set<Key,Compare,Alloc>& lhs,
                 const ft::set<Key,Compare,Alloc>& rhs )
    {
        return rhs < lhs;
    }

    template< class Key, class Compare, class Alloc >
    bool operator<=( const ft::set<Key,Compare,Alloc>& lhs,
                 const ft::set<Key,Compare,Alloc>& rhs )
    {
        return !(lhs > rhs);
    }

    template< class Key, class Compare, class Alloc >
    bool operator>=( const ft::set<Key,Compare,Alloc>& lhs,
                 const ft::set<Key,Compare,Alloc>& rhs )
    {
        return !(lhs < rhs);
    }

    template< class Key, class Compare, class Alloc >
    void swap( ft::set<Key,Compare,Alloc>& lhs, 
           ft::set<Key,Compare,Alloc>& rhs )
    {
        return lhs.swap(rhs);
    }
}