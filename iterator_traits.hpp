#pragma once

namespace ft
{
    struct	input_iterator_tag {};
	struct	output_iterator_tag {};
	struct	forward_iterator_tag : public ft::input_iterator_tag {};
	struct	bidirectional_iterator_tag : public ft::forward_iterator_tag {};
	struct	random_access_iterator_tag : public ft::bidirectional_iterator_tag {};
    
    template <class iterator>
	struct iterator_traits
	{
		typedef typename iterator::iterator_category	iterator_category;
		typedef typename iterator::value_type			value_type;
		typedef typename iterator::difference_type		difference_type;
		typedef typename iterator::pointer				pointer;
		typedef typename iterator::reference			reference;
	};

	template <class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag			iterator_category;
		typedef T									value_type;
		typedef ptrdiff_t							difference_type;
		typedef T*									pointer;
		typedef T&									reference;
	};

	template <class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag				iterator_category;
		typedef T										value_type;
		typedef ptrdiff_t								difference_type;
		typedef const T*								pointer;
		typedef const T&								reference;
	};
}