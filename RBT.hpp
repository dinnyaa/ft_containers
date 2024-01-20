#pragma once

#include <iostream>
#include "utility.hpp"
#include "bidirectional_iterator.hpp"

template< class T, class f_object, class Compare, class Allocator = std::allocator<Node<T> > >
class RBT
{ 
private:
        
    size_t _size;
    Compare compare;
    f_object access;
    Node<T>* root;
    Allocator alloc;
    Node<T>* NIL;

public:
    typedef Node<T>*                                NodePtr;
    typedef typename f_object::key_type              key_type;
    typedef ft::bidirectional_iterator<T>           iterator;
    typedef ft::bidirectional_const_iterator<T>     const_iterator;


    RBT(Compare compare = Compare())
    {
        makeNil();
        root = NIL;
        NIL->p = root;
        _size = 0;
        this->compare = compare;
    }
   
   
    RBT &operator=(const RBT &other)
    {
        if (this == &other)
            return *this;
        delete_all();
        delete_NIL();
        iterator it(other.minimum());
        makeNil();
        root = NIL;
        NIL->p = root;
        _size = 0;
        this->compare = other.compare;
        for (size_t i = 0; i < other._size; ++i, ++it)
            this->insert(*it);
        return *this;
    }

    RBT(const RBT &other)
    {
        iterator it(other.minimum());
        makeNil();
        root = NIL;
        NIL->p = root;
        _size = 0;
        this->compare = other.compare;
        for (size_t i = 0; i < other._size; ++i, ++it)
            this->insert(*it);
    }

    NodePtr successor(NodePtr n)
    {
        if (!n)
    		return n;
        if (!n->right->is_nil)
        {
            n = n->right;
            while (!n->left->is_nil)
    			n = n->left;
        }
        else if (!n->p->is_nil && n == n->p->left)
            n = n->p;
        else
        {
            while (!n->p->is_nil && n == n->p->right)
    			n = n->p;
            n = n->p;
        }
        return n;
    }

    NodePtr  lower_bound(key_type key) const
    {
        NodePtr x = root;
        NodePtr y = NIL;
        while (x != NIL)
        {
            if (!compare(access(x->key), key))
            {
                y = x;
                x = x->left;
            }
            else
                x = x->right;
        } 
        return y;
    }

    NodePtr  upper_bound(const key_type key) const
    {
        NodePtr x = root;
        NodePtr y = NIL;
        while (x != NIL)
        {
            if (compare(key, access(x->key)))
            {
                y = x;
                x = x->left;
            }
            else
                x = x->right;
        }
        return y;
    }

    void delete_NIL()
    {
        alloc.destroy(NIL);
        alloc.deallocate(NIL, 1);
    }
    
    NodePtr minimum() const
    {
        return min_helper(root);
    }

    NodePtr maximum() const
    {
        return max_helper(root);
    }

    size_t size() const
    {
        return _size;
    }

    NodePtr predecessor(NodePtr n)
    {
        if (!n)
        	return n;
        if (!n->left->is_nil)
        {
            n = n->left;
            while (!n->right->is_nil)
        		n = n->right;
        }
        else if (!n->p->is_nil && n == n->p->right)
            n = n->p;
        else
        {
            while (!n->p->is_nil && n == n->p->left)
        		n = n->p;
            n = n->p;
        }
        return n;
    }
 

    NodePtr   search(key_type key) const
    {
        NodePtr tmp = search_helper(key, root);
        return tmp;
    }

    NodePtr insert(T elem)
    {
        NodePtr z;
        Node<T> tmp (elem);
        try
        {
            z = alloc.allocate(1);
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Allocation failed: " << e.what() << '\n';
            delete_all();
            delete_NIL();
            _size = 0;

        }
        alloc.construct(z, tmp);
        return insert_helper(z, root);                                                                                                         
    }

    NodePtr insert(iterator pos, T elem)
    {
        NodePtr z;
        Node<T> tmp (elem);
        try
        {
            z = alloc.allocate(1);
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Allocation failed: " << e.what() << '\n';
            delete_all();
            delete_NIL();
            _size = 0;
        }
        alloc.construct(z, tmp);
        (void)pos;
        return insert_helper(z, root);                                                                                                         
    }

    NodePtr insert(const_iterator pos, T elem)
    {
        NodePtr z;
        Node<T> tmp (elem);
        try
        {
            z = alloc.allocate(1);
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Allocation failed: " << e.what() << '\n';
            delete_all();
            delete_NIL();
            _size = 0;
        }
        alloc.construct(z, tmp);
        (void)pos;
        return insert_helper(z, root);                                                                                                         
    }


    void delete_all()
    {
        delete_helper(root);
        root = NIL;
        NIL->p = root;
        _size = 0;
    }

    void print()
    {
        print_helper(root, 10);
    }

    void remove(key_type key)
    {
        NodePtr z = search(key);
        if (z == NIL)
            return ;
        NodePtr y = z;
        NodePtr x;
        char y_color = y->c;
        if (z->left == NIL)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if(z->right == NIL)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = min_helper(z->right);
            y_color = y->c;
            x = y->right;
            if (y->p == z)
            {
                x->p = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->c = z->c;
        }
        if (y_color == 'B')
            remove_fixup(x);
        root->c = 'B';
        _size--;
        alloc.destroy(z);
        alloc.deallocate(z, 1);
        NIL->p = root;
    }

    void print2()
    {
        inorder(root);
    }

    void swap( RBT& other )
    {
        NodePtr tmp_root = this->root;
        this->root = other.root;
        other.root = tmp_root;

        size_t tmp_size = this->_size;
        this->_size = other._size;
        other._size = tmp_size;

        Compare tmp_compare = this->compare;
        this->compare = other.compare;
        other.compare = tmp_compare;

        NodePtr tmp_nil = this->NIL;
        this->NIL = other.NIL;
        other.NIL = tmp_nil;

    }

    NodePtr getNil() const
    {
        return NIL;
    }
private:
    void makeNil()
    {
        Node<T> tmp;
        tmp.c = 'B';
        tmp.is_nil = true;
        try
        {
            NIL = alloc.allocate(1);
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Allocation failed: " << e.what() << '\n';
            delete_all();
            delete_NIL();
            _size = 0;
        }
        alloc.construct(NIL, tmp);
    }

    void transplant(NodePtr u, NodePtr v)
    {

        if (u->p == NIL)
            root = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
    }

    void print_helper(NodePtr root, int space)
    {
        if (root == NIL)
            return;
        space += 10;
        print_helper(root->right, space);
        std::cout << std::endl;
        for (int i = 10; i < space; i++)
            std::cout << " ";
        std::cout << access(root->key) << root->c << "\n";
        print_helper(root->left, space);
    }

   

    NodePtr  search_helper(key_type k, NodePtr root_ptr) const
    {
        if (root_ptr == NIL || k == access(root_ptr->key))
            return root_ptr;
        if (compare(k, access(root_ptr->key)))
            return search_helper(k, root_ptr->left);
        else
            return search_helper(k, root_ptr->right);
    }
    

    void rotateLeft(NodePtr x)
    {
        NodePtr y = x->right;
        x->right = y->left; 
        if (y->left != NIL)
            y->left->p = x;
        y->p = x->p;
        if (x->p == NIL)
            root = y;
        else if (x == x->p->left) //x is left child
            x->p->left = y;
        else                       //x is right child
            x->p->right = y;
        y->left = x;
        x->p = y;
    }

    void rotateRight(NodePtr x)
    {
        NodePtr y = x->left;
        x->left = y->right;  
        if (y->right != NIL)
            y->right->p = x;
        y->p = x->p;
        if (x->p == NIL)    
            root = y;
        else if (x == x->p->right) 
            x->p->right = y;
        else
            x->p->left = y;
        y->right = x;
        x->p = y;
    }

    void delete_helper(NodePtr node)
    {
        if (node == NIL)
            return;
        delete_helper(node->left);
        delete_helper(node->right);
        alloc.destroy(node);
        alloc.deallocate(node, 1);
    }

    NodePtr max_helper(NodePtr x) const
    {
        NodePtr tmp = x;
        while (tmp->right != NIL)
        {
            tmp = tmp->right;
        }
        return tmp;
    }

    NodePtr min_helper(NodePtr x) const
    {
        NodePtr tmp = x;
        if (tmp == NIL)
            return tmp;
        while (tmp->left != NIL)
        {
            tmp = tmp->left;
        }
        return tmp;
    }

    void insert_fixup(NodePtr z)
    {
        while (z->p->c == 'R')
        {
            if (z->p == z->p->p->left)     //x is left child
            {
                NodePtr y = z->p->p->right;
                if (y->c == 'R') //case 1
                {
                    z->p->c = 'B';
                    y->c = 'B';
                    z->p->p->c = 'R';
                    z = z->p->p;
                }
                else            //case 2, 3
                {
                    if (z == z->p->right)
                    {
                        z = z->p;
                        rotateLeft(z);
                    }
                    z->p->c = 'B';
                    z->p->p->c = 'R';
                    rotateRight(z->p->p);
                }

            }
            else        //x is right child
            {
                NodePtr y = z->p->p->left;
                if (y->c == 'R') //case 1
                {
                    z->p->c = 'B';
                    y->c = 'B';
                    z->p->p->c = 'R';
                    z = z->p->p;
                }
                else            //case 2, 3
                {
                    if (z == z->p->left)
                    {
                        z = z->p;
                        rotateRight(z);
                    }
                    z->p->c = 'B';
                    z->p->p->c = 'R';
                    rotateLeft(z->p->p);
                }
            }
        }
        root->c = 'B';
    }

    NodePtr insert_helper(NodePtr z, NodePtr& root)
    {
        NodePtr y = NIL;
        NodePtr temp = root;
        while (temp != NIL)
        {
            y = temp;
            if (compare(access(z->key), access(temp->key)))
                temp = temp->left;
            else
                temp = temp->right;
        }
        z->p = y;
        if (y == NIL)
            root = z;
        else if (compare(access(z->key), access(y->key)))
            y->left = z;
        else
            y->right = z;
        z->left = NIL;
        z->right = NIL;
        _size++;
       insert_fixup(z);
       NIL->p = root;
       return z;
    }

    void remove_fixup(NodePtr x)
    {
        while(x != root && x->c == 'B')
        {
            if(x == x->p->left)
            {
                NodePtr w = x->p->right;
                if(w->c == 'R')
                {
                    w->c = 'B';
                    x->p->c = 'R';
                    rotateLeft(x->p);
                    w = x->p->right;
                }
                if(w->left->c == 'B' && w->right->c == 'B')
                {
                    w->c = 'R';
                    x = x->p;
                }
                else
                {
                  if(w->right->c == 'B')
                  {
                    w->left->c = 'B';
                    w->c = 'R';
                    rotateRight(w);
                    w = x->p->right;
                  }
                  w->c = x->p->c;
                  x->p->c = 'B';
                  w->right->c = 'B';
                  rotateLeft(x->p);
                  x = root;
                }
          }
          else
          {
            NodePtr w = x->p->left;
            if(w->c == 'R')
            {
              w->c = 'B';
              x->p->c = 'R';
              rotateRight(x->p);
              w = x->p->left;
            }
            if(w->right->c == 'B' && w->left->c == 'B')
            {
              w->c = 'R';
              x = x->p;
            }
            else
            {
              if(w->left->c == 'B')
              {
                w->right->c = 'B';
                w->c = 'R';
                rotateLeft(w);
                w = x->p->left;
              }
              w->c = x->p->c;
              x->p->c = 'B';
              w->left->c = 'B';
              rotateRight(x->p);
              x = root;
            }
          }
        }
        x->c = 'B';
    }
};

