#pragma once

#include "iterator.hpp"
#include "random_access_iterator.hpp"


namespace ft
{

	template <typename T>
	class	reverse_iterator
	{
		public:
			typedef T    												iterator_type;
            typedef typename ft::iterator_traits<T>::iterator_category 	iterator_category;
            typedef typename ft::iterator_traits<T>::value_type      	value_type;
            typedef typename ft::iterator_traits<T>::difference_type    difference_type;
            typedef typename ft::iterator_traits<T>::pointer    		pointer;
            typedef typename ft::iterator_traits<T>::reference   		reference;

		private:
			iterator_type		Iter;

		public:

			reverse_iterator() : Iter(T()) {}

			reverse_iterator(iterator_type base) : Iter(base) {}

			template<typename U>
			reverse_iterator(const reverse_iterator<U>& rev) : Iter(rev.base()) {}

			template<typename U>
			reverse_iterator& operator=(const reverse_iterator<U>& rev)
			{
				Iter = rev.base();
				return *this;
			}

			~reverse_iterator() {}

			bool operator==(const reverse_iterator &r)const
			{
				return this->Iter == r.Iter;
			}

			bool operator!=(const reverse_iterator &r)const
			{
				return this->Iter != r.Iter;
			}

			bool operator>(const reverse_iterator &r)const
			{
				return this->Iter < r.Iter;
			}

			bool operator<(const reverse_iterator &r)const
			{
				return this->Iter > r.Iter;
			}

			bool operator>=(const reverse_iterator &r)const
			{
				return this->Iter <= r.Iter;
			}

			bool operator<=(const reverse_iterator &r)const
			{
				return this->Iter >= r.Iter;
			}

			reference operator*()const
			{
				iterator_type temp = this->Iter;
				--temp;
				return *temp;
			}

			iterator_type operator->() const
			{
				iterator_type iter(Iter);
				--iter;
				return iter;
			}

			reverse_iterator &operator++()
			{
				this->Iter--;
				return *this;
			}

			reverse_iterator operator++(int)
			{
				reverse_iterator temp;
				temp = *this;
				this->Iter--;
				return temp;
			}

			reverse_iterator &operator--()
			{
				this->Iter++;
				return *this;
			}

			reverse_iterator operator--(int)
			{
				reverse_iterator temp;
				temp = *this;
				this->Iter++;
				return temp;
			}

			reference operator[](difference_type n)const
			{
				return this->base()[-n-1];
			}

			reverse_iterator operator+=(difference_type n)
			{
				this->Iter -= n;
				return *this;
			}

			reverse_iterator operator-=(difference_type n)
			{
				this->Iter += n;
				return *this;
			}

			reverse_iterator operator+(difference_type n)
			{
				reverse_iterator temp = *this;
				temp.Iter -= n;
				return temp;
			}

			reverse_iterator operator-(difference_type n)
			{
				reverse_iterator temp = *this;
				temp.Iter += n;
				return temp;
			}

			difference_type operator-(const reverse_iterator &rhs)
			{
				return rhs.Iter - this->Iter;
			}

			iterator_type  base() const
			{
				return this->Iter;
			}
	};

	template <typename T>
    typename reverse_iterator<T>::difference_type
    operator-(const reverse_iterator<T> lhs, const reverse_iterator<T> rhs)
    {
		return lhs.base() - rhs.base();
    }

	template<typename LITER, typename RITER>
	bool operator==(const reverse_iterator<LITER>& lhs, const reverse_iterator<RITER>& rhs)
	{
		return rhs.base() == lhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator!=(const reverse_iterator<LITER>& lhs, const reverse_iterator<RITER>& rhs)
	{
		return rhs.base() != lhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator<(const reverse_iterator<LITER>& lhs, const reverse_iterator<RITER>& rhs)
	{
		return rhs.base() < lhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator<=(const reverse_iterator<LITER>& lhs, const reverse_iterator<RITER>& rhs)
	{
		return rhs.base() <= lhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator>(const reverse_iterator<LITER>& lhs, const reverse_iterator<RITER>& rhs)
	{
		return rhs.base() > lhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator>=(const reverse_iterator<LITER>& lhs, const reverse_iterator<RITER>& rhs)
	{
		return rhs.base() >= lhs.base();
	}

	template<typename Iter>
	typename reverse_iterator<Iter>::difference_type operator-(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{
		return rhs.base() - lhs.base();
	}

	template <typename T>
	reverse_iterator<T> operator+(typename reverse_iterator<T>::difference_type lhs, reverse_iterator<T> &rhs)
	{
		return rhs + lhs;
	}
	

}
