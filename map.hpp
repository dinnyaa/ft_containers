#pragma once
#include <iostream>
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "bidirectional_iterator.hpp"
#include "exception"
#include "algorithm.hpp"
#include <new>
#include "RBT.hpp"

namespace ft
{

    template<
        class Key,
        class T,
        class Compare = std::less<Key>,
        class Allocator = std::allocator<ft::pair<const Key, T> >
    > class map
    {
    public:

        typedef Key                                             key_type;
        typedef T                                               mapped_value;
        typedef ft::pair<const Key, T>                          value_type;
        typedef std::size_t                                     size_type;
        typedef std::ptrdiff_t                                  difference_type;
        typedef Compare                                         key_compare;
        typedef Allocator                                       allocator_type;
        typedef value_type&                                     reference;
        typedef const value_type&                               const_reference;
        typedef typename Allocator::pointer                     pointer;
        typedef typename Allocator::const_pointer               const_pointer;
        typedef ft::bidirectional_iterator<value_type>          iterator;
        typedef ft::bidirectional_const_iterator<value_type>    const_iterator;
        typedef ft::reverse_iterator<iterator>                  reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>            const_reverse_iterator;

    private:

        template <typename Type>
        struct SelectFirst
        {
            typedef typename Type::first_type key_type;
            key_type &operator()(Type &val)
            {
                return val.first;
            }
            const key_type &operator()(Type &val) const
            {
                return val.first;
            }
        };

        typedef  Node<value_type>*                          NodePtr;

        RBT<value_type, SelectFirst<value_type>, Compare>   _tree;
        allocator_type                                      _alloc;

        public:
            /*              Constructors            */

            map() :_tree(Compare()) {}

            explicit map( const Compare& comp, 
              const Allocator& alloc = Allocator()) : _tree(comp), _alloc(alloc){}

            explicit map( const Allocator& alloc )  :_tree(Compare()), _alloc(alloc) {}

            template< class InputIt >
            map( InputIt first, InputIt last, const Compare& comp = Compare(),
                const Allocator& alloc = Allocator(), typename ft::enable_if <!ft::is_integral<InputIt>::value, bool>::type = 0)
                :_tree(comp), _alloc(alloc)
            {
                while (first != last)
                {
                    insert(*first);
                    ++first;
                }
            }

            map( const map& other )
            : _tree(other._tree) {}

            /*             Destructor          */

            ~map()
            {
                _tree.delete_all();
                _tree.delete_NIL();
            }

            map& operator=( const map& other )
            {
                _tree = other._tree;
                return *this;
            }


            class value_compare : std::binary_function<value_type, value_type, bool>
			{
				public:
					value_compare(key_compare c) : _comp(c) {}

					bool	operator()(const value_type &lhs, const value_type &rhs) const
					{
						return (this->_comp(lhs.first, rhs.first));
					}

				protected:
					key_compare	_comp;

				private:
					friend class	map<key_type, value_type, key_compare, allocator_type>;
			};

            allocator_type get_allocator() const
            {
                return _alloc;
            }

            /*             Element access           */
            T& at( const Key& key )
            {
                NodePtr ret = _tree.search(key);
                if (ret == _tree.getNil())
                    throw  std::out_of_range("map::at");
                else
                    return (ret->key.second);            
            }

            const T& at( const Key& key ) const
            {
                NodePtr ret = _tree.search(key);
                if (ret == _tree.getNil())
                    throw  std::out_of_range("map::at");
                else
                    return const_reference(ret->key.second);               
            }

            T& operator[]( const Key& key )
            {
                return insert(ft::make_pair(key, T())).first->second;
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

            /*             Capacity                */

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
                NodePtr tmp = _tree.search(value.first);
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
                NodePtr tmp = _tree.search(value.first);
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
                    if(_tree.search(first->first) == _tree.getNil())
                        _tree.insert(*first);
                    ++first;
                }
            }

            void erase( iterator pos )
            {
                _tree.remove(pos->first);
            }

            void erase( iterator first, iterator last )
            {
                NodePtr tmp = first.base();
                value_type last_pair = *last;

                while (tmp->key != last_pair)
                {
                   _tree.remove(tmp->key.first);
                    tmp = _tree.upper_bound(tmp->key.first);
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

            void swap( map& other )
            {
                this->_tree.swap(other._tree);
            }

            /*              Observers               */

            key_compare key_comp() const
            {
                return key_compare();
            }

            value_compare value_comp() const
            {
                return (value_compare(key_compare()));
            }
    };

    template< class Key, class T, class Compare, class Alloc >
    bool operator==( const ft::map<Key,T,Compare,Alloc>& lhs,
                 const ft::map<Key,T,Compare,Alloc>& rhs )
    {
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class Key, class T, class Compare, class Alloc >
    bool operator!=( const ft::map<Key,T,Compare,Alloc>& lhs,
                 const ft::map<Key,T,Compare,Alloc>& rhs )
    {
        return !(lhs == rhs);
    }

    template< class Key, class T, class Compare, class Alloc >
    bool operator<( const ft::map<Key,T,Compare,Alloc>& lhs,
                const ft::map<Key,T,Compare,Alloc>& rhs )
    {
         return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class Key, class T, class Compare, class Alloc >
    bool operator>( const ft::map<Key,T,Compare,Alloc>& lhs,
                 const ft::map<Key,T,Compare,Alloc>& rhs )
    {
        return rhs < lhs;
    }

    template< class Key, class T, class Compare, class Alloc >
    bool operator<=( const ft::map<Key,T,Compare,Alloc>& lhs,
                 const ft::map<Key,T,Compare,Alloc>& rhs )
    {
        return !(lhs > rhs);
    }

    template< class Key, class T, class Compare, class Alloc >
    bool operator>=( const ft::map<Key,T,Compare,Alloc>& lhs,
                 const ft::map<Key,T,Compare,Alloc>& rhs )
    {
        return !(lhs < rhs);
    }

    template< class Key, class T, class Compare, class Alloc >
    void swap( ft::map<Key,T,Compare,Alloc>& lhs, 
           ft::map<Key,T,Compare,Alloc>& rhs )
    {
        return lhs.swap(rhs);
    }
}