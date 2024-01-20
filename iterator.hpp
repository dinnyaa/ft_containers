#pragma once
#include  <cstddef>
#include "iterator_traits.hpp"


namespace ft
{

    template<class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
    {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference	reference;
    };

    
	template<class It>
	typename ft::iterator_traits<It>::difference_type 
    do_distance(It first, It last, ft::input_iterator_tag)
	{
	    typename ft::iterator_traits<It>::difference_type result = 0;
	    while (first != last) {
	        ++first;
	        ++result;
	    }
	    return result;
	}
 
	template<class It>
	typename ft::iterator_traits<It>::difference_type 
	do_distance(It first, It last, ft::random_access_iterator_tag)
	{
	    return last - first;
	}

	template<class It>
	typename ft::iterator_traits<It>::difference_type 
	do_distance(It first, It last, std::random_access_iterator_tag)
	{
	    return do_distance(first, last, ft::random_access_iterator_tag());
	}

	template<class It>
	typename ft::iterator_traits<It>::difference_type 
	do_distance(It first, It last, std::input_iterator_tag)
	{
	    return do_distance(first, last, ft::input_iterator_tag());
	}

	
	
	template<class It>
	typename ft::iterator_traits<It>::difference_type 
	distance(It first, It last)
	{
	    return ft::do_distance(first, last, typename ft::iterator_traits<It>::iterator_category());
	}

}