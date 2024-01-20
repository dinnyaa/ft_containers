#pragma once
#include "iterator.hpp"

namespace ft
{

    template <typename T>
    class random_access_iterator : public ft::iterator <ft::random_access_iterator_tag, T> 
    {
        T* ptr;
    public:
        typedef ft::random_access_iterator<const T> const_iterator;
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::value_type value_type;
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::pointer pointer;
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::difference_type difference_type;
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::reference reference;
        typedef typename ft::iterator<ft::random_access_iterator_tag, T>::iterator_category iterator_category;


        random_access_iterator() : ptr(NULL) {}

        random_access_iterator(T* addr) : ptr(addr) {}

        random_access_iterator(const random_access_iterator &other) : ptr(other.ptr) {}

        random_access_iterator &operator=(const random_access_iterator &other)
        {
            if (this == &other)
                return *this;
            ptr = other.ptr;
            return *this;
        }

        operator const_iterator()
        {
            return const_iterator(ptr);
        }

        ~random_access_iterator() {}

        pointer base() const
        {
            return ptr;
        }


        random_access_iterator operator+(difference_type n) const
        {
            return(random_access_iterator(ptr + n));
        }

        random_access_iterator operator-(difference_type n) const
        {
            return(random_access_iterator(ptr - n));
        }

        reference operator* () const
        {
            return *ptr;
        }

        pointer operator-> () const 
        {
            return ptr;
        }


        reference	operator[](difference_type n) const
		{
			return (this->ptr[n]);
		}

        random_access_iterator &operator++()
        {
            ++ptr;
            return *this;
        }

        random_access_iterator operator++(int)
        {
            random_access_iterator<T> temp(*this);
            this->ptr++;
            return temp;
        }

        random_access_iterator &operator--()
        {
            --ptr;
            return *this;
        }

        random_access_iterator operator--(int)
        {
            random_access_iterator temp(*this);
            this->ptr--;
            return temp;
        }

        random_access_iterator &operator+=(difference_type n)
        {
            ptr+=n;
            return (*this);
        }

        random_access_iterator &operator-=(difference_type n)
        {
            ptr-=n;
            return (*this);
        }

        friend difference_type operator-(const random_access_iterator &lhs, const random_access_iterator &rhs)
        {
            difference_type a = 0;
            a = (lhs.ptr - rhs.ptr);
            return (a);
        }

        friend random_access_iterator operator+(const difference_type n, const random_access_iterator &rhs)
        {
            return (random_access_iterator(rhs.ptr + n));
        }

        bool operator==(const random_access_iterator &rhs) const
        {
            return (this->ptr == rhs.ptr);
        }

        bool operator!=(const random_access_iterator &rhs) const
        {
            return (this->ptr != rhs.ptr);
        }

        bool operator<(const random_access_iterator &rhs) const
        {
            return (this->ptr < rhs.ptr);
        }

        bool operator<=(const random_access_iterator &rhs) const
        {
            return (this->ptr <= rhs.ptr);
        }

        bool operator>(const random_access_iterator &rhs) const
        {
            return (this->ptr > rhs.ptr);
        }

        bool operator>=(const random_access_iterator &rhs) const
        {
            return (this->ptr >= rhs.ptr);
       }
    };

    template<typename LITER, typename RITER>
	bool operator==(const random_access_iterator<LITER>& lhs, const random_access_iterator<RITER>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator!=(const random_access_iterator<LITER>& lhs, const random_access_iterator<RITER>& rhs)
	{
		return lhs.base() != rhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator<(const random_access_iterator<LITER>& lhs, const random_access_iterator<RITER>& rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator<=(const random_access_iterator<LITER>& lhs, const random_access_iterator<RITER>& rhs)
	{
		return lhs.base() <= rhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator>(const random_access_iterator<LITER>& lhs, const random_access_iterator<RITER>& rhs)
	{
		return lhs.base() > rhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator>=(const random_access_iterator<LITER>& lhs, const random_access_iterator<RITER>& rhs)
	{
		return lhs.base() >= rhs.base();
	}

};