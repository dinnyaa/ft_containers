#pragma once

#include "iterator.hpp"
#include "iterator_traits.hpp"

template <typename Key>
struct Node
{
    typedef Node<Key>*  NodePtr;
    typedef Key         value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
            
    Key     key;
    Node*   p;
    Node*   left;
    Node*   right;
    char    c;
    bool    is_nil;


    Node(Key _key = Key(), Node *_p = NULL, Node *_left = NULL, Node *_right = NULL)
        : key(_key), p(_p), left(_left), right(_right), c('R'), is_nil(false)   {}
};

template <typename T>
static Node<T>* min_helper(Node<T>* x)
    {
        Node<T>* tmp = x;
        if (tmp->is_nil)
            return tmp;
        while (!tmp->left->is_nil)
        {
            tmp = tmp->left;
        }
        return tmp;
    }


template <typename T>
Node<T>* max_helper(Node<T>* x)
{
    Node<T>* tmp = x;
    if (tmp->is_nil)
            return tmp;
    while (!tmp->right->is_nil)
    {
        tmp = tmp->right;
    }
    return tmp;
}

template <typename T>
static Node<T>* increment(Node<T>* node)
{
    Node<T>* base = node;
    if (base->is_nil)
    {
        base = base->p;
        base = min_helper(base);
    }
    else if (base->right && !base->right->is_nil)
			base = min_helper(base->right);
		else
		{
			 Node<T>* node = base->p;
			while (!base->is_nil && base == node->right)
			{
				base = node;
				node = node->p;
			}
			base = node;
		}
		return base;
}

template <typename T>
static Node<T>* decrement(Node<T>* node)
{
	Node<T>* _base = node;
	if (_base->is_nil)
	{
		_base = _base->p;
		_base = max_helper(_base);
	}
	else if (_base->left && !_base->left->is_nil)
		_base = max_helper(_base->left);
	else
	{
		Node<T>* node = _base->p;
		while (!_base->is_nil && _base == node->left)
		{
			_base = node;
			node = node->p;
		}
		_base = node;
	}
	return _base;
}


namespace ft
{
    template <typename T>
    class bidirectional_iterator : public ft::iterator <ft::bidirectional_iterator_tag, T> 
    {
        typedef   Node<T>*                                                                  node_ptr;
        node_ptr ptr;

    public:
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::iterator_category iterator_category;
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::value_type        value_type;
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::pointer           pointer;
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::reference         reference;

        bidirectional_iterator() : ptr (NULL) {}

        bidirectional_iterator(node_ptr addr) : ptr(addr) {}

        bidirectional_iterator(const bidirectional_iterator &other) : ptr(other.ptr) {}

        bidirectional_iterator &operator=(const bidirectional_iterator &other)
        {
            if (this == &other)
                return *this;
            ptr = other.ptr;
            return *this;
        }

        ~bidirectional_iterator() {}

        node_ptr base()
        {
            return ptr;
        }

        const node_ptr base() const
        {
            return ptr;
        }

        T& operator* () const
        {
            return (ptr->key);
        }

        T* operator-> () const 
        {
            return &(ptr->key);
        }

        bidirectional_iterator &operator++()
        {
            ptr = increment(ptr);
            return *this;
        }

        bidirectional_iterator operator++(int)
        {
            bidirectional_iterator<T> temp(*this);
            ptr = increment(ptr);
            return temp;
        }

        bidirectional_iterator &operator--()
        {
            ptr = decrement(ptr);
            return *this;
        }

        bidirectional_iterator operator--(int)
        {
            bidirectional_iterator temp(*this);
            ptr = decrement(ptr);
            return temp;
        }

        bool operator==(const bidirectional_iterator &rhs) const
        {
            return (this->ptr == rhs.ptr);
        }

        bool operator!=(const bidirectional_iterator &rhs) const
        {
            return (this->ptr != rhs.ptr);
        }

   
    };

    template<typename LITER, typename RITER>
	bool operator==(const bidirectional_iterator<LITER>& lhs, const bidirectional_iterator<RITER>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename LITER, typename RITER>
	bool operator!=(const bidirectional_iterator<LITER>& lhs, const bidirectional_iterator<RITER>& rhs)
	{
		return lhs.base() != rhs.base();
	}


    template <typename T>
    class bidirectional_const_iterator : public ft::iterator <ft::bidirectional_iterator_tag, T> 
    {
        typedef   Node<T>*                                                                  node_ptr;
        node_ptr ptr;

    public:
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, const T>::iterator_category       iterator_category;
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, const T>::value_type              value_type;
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, const T>::pointer           pointer;
        typedef typename ft::iterator<ft::bidirectional_iterator_tag, const T>::reference         reference;

        bidirectional_const_iterator() : ptr (NULL) {}

        bidirectional_const_iterator(const node_ptr addr) : ptr(addr) {}

        bidirectional_const_iterator(const bidirectional_const_iterator &other) : ptr(other.ptr) {}

        bidirectional_const_iterator(bidirectional_iterator<T> other) : ptr(other.base()) {}


        bidirectional_const_iterator &operator=(const bidirectional_const_iterator &other)
        {
            if (this == &other)
                return *this;
            ptr = other.ptr;
            return *this;
        }


        ~bidirectional_const_iterator() {}

        node_ptr base() const
        {
            return ptr;
        }

        const T& operator* () const
        {
            return (ptr->key);
        }

        const T* operator-> () const 
        {
            return &(ptr->key);
        }

        bidirectional_const_iterator &operator++()
        {
            ptr = increment(ptr);
            return *this;
        }

        bidirectional_const_iterator operator++(int)
        {
            bidirectional_const_iterator<T> temp(*this);
            ptr = increment(ptr);
            return temp;
        }

        bidirectional_const_iterator &operator--()
        {
            ptr = decrement(ptr);
            return *this;
        }

        bidirectional_const_iterator operator--(int)
        {
            bidirectional_const_iterator temp(*this);
            ptr = decrement(ptr);
            return temp;
        }

        bool operator==(const bidirectional_const_iterator &rhs) const
        {
            return (this->ptr == rhs.ptr);
        }

        bool operator!=(const bidirectional_const_iterator &rhs) const
        {
            return (this->ptr != rhs.ptr);
        }

  
    };


}
