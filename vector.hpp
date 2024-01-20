#pragma once

#include <iterator>
#include <stdexcept>
#include <exception>
#include "type_traits.hpp"
#include "random_access_iterator.hpp"
#include "reverse_iterator.hpp"
#include "algorithm.hpp"
#include <iostream>

namespace ft
{
    template <typename T>
    void swap(T &a1, T &a2)
    {
        a1.swap(a2);
    }

    template<class T, class Allocator = std::allocator<T>
     >class vector
    {
        public:
            typedef T										value_type;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;
            
            typedef Allocator								allocator_type;
			typedef typename Allocator::pointer 			pointer;
			typedef typename Allocator::const_pointer 	    const_pointer;
            
			typedef std::size_t								size_type;
			typedef std::ptrdiff_t							difference_type;

			typedef ft::random_access_iterator<T> 		    iterator;
			typedef ft::random_access_iterator<const T> 	const_iterator;
			typedef ft::reverse_iterator<iterator> 		    reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

            /*                          constructors                    */
            vector()
            : _ptr(NULL), _capacity(0), _size(0), _allocator(Allocator()) {}

            explicit vector( const Allocator& alloc )
            : _ptr(NULL), _capacity(0), _size(0), _allocator(alloc) {}

            explicit vector( size_type count, const T& value = T(), const Allocator& alloc = Allocator())
            : _ptr(NULL), _capacity(0), _size(0), _allocator(alloc)
            {
                _capacity = count;
                _size = count;
                try
                {
                    _ptr = _allocator.allocate(count);
                    for (size_type i = 0; i < _size; i++)
                        _allocator.construct(_ptr + i, value);     
                }
                catch(...)
                {
                    _capacity = 0;
                    _size = 0;
                    _ptr = NULL;
                    throw;
                }
                
            }

            template< class InputIt >
            vector( InputIt first, InputIt last, const Allocator& alloc = Allocator(), typename enable_if <!is_integral<InputIt>::value, bool>::type = 0)
            : _ptr(NULL), _capacity(0), _size(0), _allocator(alloc)
            {
                difference_type n = ft::distance(first, last);
                _capacity = n / sizeof(T);
                try
                {
                    _ptr = n == 0 ? NULL : _allocator.allocate(_capacity);
                }
                catch(const std::exception& e)
                {
                    _capacity = 0;
                    _size = 0;
                    _ptr = NULL;
                    throw;
                }
                InputIt it = first;
                while (it != last)
                {
                    push_back(*it);
                    ++it;
                }                
            }

            vector( const vector& other )
            : _capacity(other._capacity), _size(other._size), _allocator(other._allocator)
            {
                try
                {
                    _ptr = _allocator.allocate(_capacity);
                    for (size_type i = 0; i < _size; i++)
                        _allocator.construct(_ptr + i, other._ptr[i]);
                }
                catch(...)
                {
                    _capacity = 0;
                    _size = 0;
                    _ptr = NULL;
                    throw;
                }
            }

            void assign( size_type count, const T& value )
            {
                this->clear();
                try
                {
                   reserve(count);
                    for (size_type i = 0; i < count; i++)
                        _allocator.construct(_ptr + i, value);
                    _size = count;
                }
                catch(...)
                {
                    _size = 0;
                    _capacity = 0;
                    _ptr = NULL;
                    throw;
                }
            }
	
            template< class InputIt >
            void assign(InputIt first, InputIt last, 
                typename ft::enable_if<!ft::is_integral<InputIt>::value, bool>::type* = 0)
            {
                this->clear();
                while (first != last)
                {
                    this->push_back((*first));       
                    ++first;
                }
               
            }

            /*                          destructor                 */
            virtual ~vector()
            {
                for(size_type i = 0; i < _size; i++)
                    _allocator.destroy(_ptr + i);
                _allocator.deallocate(_ptr, _capacity);
            }

            allocator_type get_allocator() const
            {
                return allocator_type();
            }

            vector& operator=( const vector& other )
            {
                if (this == &other)
                    return *this;
                clear();
                _allocator.deallocate(_ptr, _capacity);
                _capacity = other._capacity;
                _size = other._size;
                try
                {
                    _ptr = _allocator.allocate(_capacity);
                }
                catch(const std::exception& e)
                {
                    _size = 0;
                    _capacity = 0;
                    _ptr = NULL;
                    throw;
                }
                
                for (size_type i = 0; i < _size; i++)
                    _allocator.construct(_ptr + i, other._ptr[i]);
                return *this;
                
            }

        /*                      element access                  */
        reference at( size_type pos )
        {
            if (pos > _size)
                throw  std::out_of_range("vector::at() - Index out of range");
            return _ptr[pos];
        }

        const_reference at( size_type pos ) const
        {
            if (pos > _size)
                throw  std::out_of_range("vector::at() - Index out of range");
            return (const_reference)_ptr[pos];
        }

        reference operator[]( size_type pos )
        {
            return _ptr[pos];
        }

        const_reference operator[]( size_type pos ) const
        {
            return (const_reference)_ptr[pos];
        }

        reference front()
        {
            return _ptr[0];
        }

        const_reference front() const
        { 
            return (const_reference)_ptr[0];
        }

         reference back()
        {
            return _ptr[_size - 1];
        }

        const_reference back() const
        {
            return (const_reference)_ptr[_size - 1];
        }

        T* data()
        {
            return _ptr;
        }

        const T* data() const
        {
            return (const_pointer)_ptr;
        }

        // /*                          Capacity                  */
        bool empty() const
        {
            return begin() == end();
        }

        size_type size() const
        {
            return _size;
        }

        size_type max_size() const
        {
            return _allocator.max_size();
        }

        size_type capacity() const
        {
            return _capacity;
        }


        // /*                  Modifiers                  */
        void clear()
        {
            for (size_type i = 0; i < _size; i++)
                _allocator.destroy(_ptr + i);
            _size = 0;
        }

        iterator insert( const_iterator pos, const T& value )
        {
            return (insert(pos, 1, value)); 
        } 

        iterator insert( const_iterator pos, size_type count, const T& value )
        {
            if (_size == _capacity && _size == 0)
            {
                for (size_t i = 0 ; i < count; i++)
                    push_back(value); 
                return (iterator(_ptr));
            }
            size_type old_cap = _capacity;
            size_type dist = (size_type)ft::distance(const_iterator(begin()), pos);
            T* tmp_ptr;
            if (count + _size > _capacity)
                _capacity = count + _size;
            try
            {
                tmp_ptr = this->_allocator.allocate(_capacity);
            }
            catch(...)
            {
                for(size_type j = 0; j < _size; j++)
                    this->_allocator.destroy(_ptr + j);
                _allocator.deallocate(_ptr, old_cap);
                _size = 0;
                _capacity = 0;
                _ptr = NULL;
                throw;
            }
            size_type i = 0, tmp_i = 0;

            for (; i < dist; tmp_i++, i++)
                this->_allocator.construct(tmp_ptr + tmp_i, _ptr[i]);

            for (; tmp_i < dist + count; tmp_i++)
                this->_allocator.construct(tmp_ptr + tmp_i, value);

            for (; i <  _size; i++, tmp_i++)
                this->_allocator.construct(tmp_ptr + tmp_i, _ptr[i]);
            for(size_type j = 0; j < _size; j++)
                this->_allocator.destroy(_ptr + j);
            _size += count;
            _allocator.deallocate(_ptr, old_cap);
            _ptr = tmp_ptr;
            return iterator(_ptr + dist);
        }
	
        template< class InputIt >
        iterator insert( const_iterator pos, InputIt first, InputIt last,
                        typename enable_if <!is_integral<InputIt>::value, bool>::type = 0)
        {
            size_type old_cap = _capacity;
            size_type pos_index = (size_type)ft::distance(const_iterator(begin()), pos);
            size_type dist = (size_type)ft::distance(first, last);
            T* tmp_ptr;
            if (dist + _size > _capacity)
                _capacity = dist + _size;
            try
            {
                tmp_ptr = this->_allocator.allocate(_capacity);
            }
            catch(...)
            {
                for(size_type j = 0; j < _size; j++)
                    this->_allocator.destroy(_ptr + j);
                _allocator.deallocate(_ptr, old_cap);
                _ptr = NULL;
                _capacity = 0;
                _size = 0;
                throw;
            }
            size_type i = 0, tmp_i = 0;
            for (; i < pos_index; tmp_i++, i++)
                this->_allocator.construct(tmp_ptr + tmp_i, _ptr[i]);
            try{
            for (; first != last; tmp_i++, first++)
                this->_allocator.construct(tmp_ptr + tmp_i, *first);
            }
            catch(...) 
            {
                for (size_type k = 0; k < tmp_i; k++)
                    this->_allocator.destroy(tmp_ptr + k);
                this->_allocator.deallocate(tmp_ptr, _capacity);
                for(size_type j = 0; j < _size; j++)
                this->_allocator.destroy(_ptr + j);
                 _allocator.deallocate(_ptr, old_cap);
                 _size = 0 ;
                 _capacity = 0;
                 throw;
            }
            for (; i <  _size; i++, tmp_i++)
                this->_allocator.construct(tmp_ptr + tmp_i, _ptr[i]);
            for(size_type j = 0; j < _size; j++)
                this->_allocator.destroy(_ptr + j);
            _size += dist;
            _allocator.deallocate(_ptr, old_cap);
            _ptr = tmp_ptr;
            return iterator(_ptr + dist);
        }

        iterator erase( iterator pos )
        {
            return (erase(pos, pos + 1));
        }

        iterator erase(iterator first, iterator last)
        {
            size_type i = 0, m = 0;
            while((_ptr + i) != first.base())
                i++;
            int j = i;
            m = i;
            while (first != last)
            {
                _allocator.destroy(_ptr + i);
                ++first;
                i++;
            }
            while (i < _size)
            {
                _allocator.construct(_ptr + j, _ptr[i]);
                _allocator.destroy(_ptr + i);
                i++;
                j++;
            }
            _size = j;
            return (iterator(_ptr + m));
        }

        void push_back( const T& value )
        {
            try
            {   
                if(_size == _capacity )
                {
                    size_t tmp = (_capacity == 0) ? 1 : _capacity * 2;
                    reserve(tmp);
                }
                _allocator.construct(_ptr + _size, value);
                _size++;

            }
            catch(const std::exception& e)
            {
                _ptr = NULL;
                _size = 0;
                _capacity = 0;
                std::cerr << e.what() << '\n';
                throw;
            }
            
        }

        void pop_back()
        {
            _allocator.destroy(_ptr + _size);
            _size--;
        }

        void resize( size_type count, T value = T() )
        {
            if (count < _size)
                while (_size != count)
                    pop_back();
            else if (count > _size)
                while (_size != count)
                    push_back(value);
        }

        void reserve( size_type new_cap )
        {
                pointer temp_arr = NULL;
				size_type temp_size = 0;

				if (this->_capacity == this->max_size() || new_cap > this->max_size())
					 std::length_error(std::string("vector::reserve() - Not enough memory"));
				if (new_cap > this->_capacity)
				{
					try
					{
						temp_arr = this->_allocator.allocate(new_cap);
						for(temp_size = 0; temp_size < this->_size; ++temp_size)
							this->_allocator.construct(temp_arr + temp_size, this->_ptr[temp_size]);
					}
					catch(...)
					{
						for(size_type i = 0; i < temp_size; ++i)
							this->_allocator.destroy(temp_arr + i);
						this->_allocator.deallocate(temp_arr, new_cap);
                        for (size_type k = 0; k < this->_size; ++k)
						    this->_allocator.destroy(this->_allocator.address(this->_ptr[k]));
					    this->_allocator.deallocate(this->_ptr, this->_capacity);
                        throw;
					}
					for (size_type k = 0; k < this->_size; ++k)
						this->_allocator.destroy(this->_allocator.address(this->_ptr[k]));
					this->_allocator.deallocate(this->_ptr, this->_capacity);
					this->_ptr = temp_arr;
					this->_capacity = new_cap;
					this->_size = temp_size;
				}
        }

        iterator begin()
        {
            return iterator(_ptr);
        }

        const_iterator begin() const
        {
                return const_iterator(_ptr);
        }

        iterator end()
        {
            return iterator(_ptr + _size);
        }

        const_iterator end() const
        {
                return const_iterator(_ptr + _size);
        }

        reverse_iterator rbegin()
        {
            return iterator(end());
        }

        const_reverse_iterator rbegin() const
        {
                return const_reverse_iterator(end());
        }

        reverse_iterator rend()
        {
            return iterator(begin());
        }

        const_reverse_iterator rend() const
        {
                return const_reverse_iterator(begin());
        }

        void swap(vector &other)
        {
            ft::swap(this->_size, other._size);
            ft::swap(this->_capacity, other._capacity);
            ft::swap(this->_allocator, other._allocator);
            ft::swap(this->_ptr, other._ptr);

        }

        private:
			pointer			_ptr;
            size_type 		_capacity;
            size_type 		_size;
            allocator_type	_allocator;

    };

    template <typename T>
    void swap(vector<T> &v1, vector<T> &v2)
    {
        v1.swap(v2);
    }

    template <typename T>
    bool operator<(const vector<T> &lhs, const vector<T> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <typename T>
    bool operator>(const vector<T> &lhs, const vector<T> &rhs) 
    {
        return rhs < lhs;
    }

    template <typename T>
    bool operator>=(const vector<T> &lhs, const vector<T> &rhs)
    {
       return !(lhs < rhs);
    }

    template <typename T>
    bool operator<=(const vector<T> &lhs, const vector<T> &rhs)
    {
        return !(lhs > rhs);
    }

    template <typename T>
    bool operator==(const vector<T> &lhs, const vector<T> &rhs)
    {
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <typename T>
    bool operator!=(const vector<T> &lhs, const vector<T> &rhs)
    {
        return !(lhs == rhs);
    }

}
