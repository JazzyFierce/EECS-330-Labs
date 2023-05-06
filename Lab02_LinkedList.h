#ifndef _MY_LINKEDLIST_H_
#define _MY_LINKEDLIST_H_

#include <algorithm>
#include <iostream>

template <typename DataType>
class MyLinkedList
{
  private:
    
    struct Node 
    {
        DataType  data;
        Node   *prev;
        Node   *next;
        
        Node(const DataType &d = DataType{ }, Node *p = nullptr, Node *n = nullptr) : 
        data{d}, 
        prev{p}, 
        next{n} 
        { }

        Node(DataType&& d, Node* p = nullptr, Node* n = nullptr) : 
        data{std::move(d)}, 
        prev{p}, 
        next{n} 
        { }
    }; 

    int theSize;        // the number of elements that the linked list is currently holding
    Node *head;         // pointer to the head node; does not hold real data
    Node *tail;         // pointer to the tail note; does not hold real data

    void init( )
    { 
        head = new Node;
        tail = new Node;
        head->next=tail;
        tail->prev=head;
        theSize=0;
    }

  public:

    // define the const_iterator class
    class const_iterator 
    { 
      protected:
        Node *current;
        // returns a mutable object
        // defined as proected as we don't expect it to be called directly by other classess
        DataType& retrieve() const
        { return current->data; }
        // constructor to be hidden from other classes; 
        // we don't expect direct construction of iterator form pointer
        const_iterator(Node *p) : 
        current{p} 
        {}
        
        friend class MyLinkedList<DataType>;

      public:
        const_iterator() : 
        current{nullptr} 
        {}

        // add const to the return type and make the return value by retrieve() non-mutable
        const DataType& operator* () const
        { return retrieve(); }
        
        // move to the next data element
        // the prefix increment (e.g. ++ p) with 0 parameter
        const_iterator& operator++ ()
        {
            current = current->next;
            return *this;
        }

        // move to the next data element
        // the suffix increment (e.g. p ++) with 1 int parameter
        const_iterator operator++ ( int )
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        // move to the previous data element
        // the prefix decrement (e.g. -- p) with 0 parameter
        const_iterator& operator-- ()
        {
            current = current->prev;
            return *this;
        }

        // move to the previous data element
        // the suffix decrement (e.g. p --) with 1 int parameter
        const_iterator operator-- (int)
        {
            const_iterator old = *this;
            --(this);
            return old;
        }

        // check whether two iterators point to the same data element
        bool operator== (const const_iterator& rhs) const
        { return (current == rhs.current); }
        
        // check whether two iterators point to the same data element
        bool operator!= (const const_iterator & rhs) const
        { return !(*this == rhs); }
      
    }; 

    // define the iterator class; inherit from class const_iterator
    class iterator : public const_iterator //{{{2
    {
      protected:
        iterator(Node *p) : 
        const_iterator{p}
        {}
        
        friend class MyLinkedList<DataType>;
      
      public:
        // default constructor
        iterator()
        {}

        // the return type is mutable
        DataType& operator* ()
        { return const_iterator::retrieve(); }
        
        // keep the return type as non-mutable
        const DataType& operator* () const
        { return const_iterator::retrieve(); }

        iterator & operator++ ()
        {
            this->current = this->current->next;
            return *this;
        }

        iterator operator++ (int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator operator-- ()
        {
            this->current = this->current->prev;
            return *this;
        }
        
        iterator operator-- (int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }

        // don't need to overload operator= and operator!= as they should behave the same as in const_iterator
    }; 

  // defining the MyLinkedList class methods
  public:

    // default constructor
    MyLinkedList()
    { init(); }
  
    // copy constructor
    MyLinkedList(const MyLinkedList& rhs)
    { 
        init();
        for(auto &x : rhs)
          push_back(x);
    }

    // move constructor
    MyLinkedList (MyLinkedList&& rhs) :
    theSize(rhs.theSize),
    head(rhs.head),
    tail(rhs.tail)
    { 
        rhs.theSize = 0;
        rhs.head = nullptr;
        rhs.tail = nullptr;
    }

    // destructor
    ~MyLinkedList()
    {
        clear();
        delete head;
        delete tail;
    }

    // copy assignment
    MyLinkedList & operator= (const MyLinkedList& rhs)
    { 
        MyLinkedList copy = rhs;
        std::swap(*this,copy);
        return *this;
    }
  
    // move assignment
    MyLinkedList & operator= (MyLinkedList && rhs)
    { 
        std::swap(tail, rhs.tail);
        std::swap(head, rhs.head);
        std::swap(theSize, rhs.theSize);

        return *this;
    }

    // iterator interface
    iterator begin()
    { return {head->next}; }

    const_iterator begin() const
    { return {head->next}; }
  
    iterator end()
    { return {tail}; }

    const_iterator end() const
    { return {tail}; }
  
    // gets the size of the linked list
    int size() const
    { return theSize; }

    // checks if the linked list is empty; return True if empty, false otherwise
    bool empty() const
    { return (size() == 0); }

    // deletes all nodes excepts the head and tail
    void clear()
    {
        while(!empty())
          pop_front();
    }

    // return the first data element as mutable
    DataType& front()
    { return *begin(); }

    // return the first data element as non-mutable
    const DataType& front() const
    { return head->next->data; }

    // return the last data element as mutable
    DataType& back()
    { return *--end(); }

    // return the last data element as non-mutable
    const DataType& back() const
    { return *--end(); }

    // insert x before itr; return iterator pointing to the newly inserted data element
    iterator insert(iterator itr, const DataType& x)
    {
        Node *p = itr.current;
        theSize++;

        return {p->prev =  p->prev->next = new Node{x, p->prev, p}};
    }

    // insert x before itr; return iterator pointing to the newly inserted data element
    iterator insert(iterator itr, DataType&& x)
    {
        Node *p = itr.current;
        theSize++;
        return {p->prev =  p->prev->next = new Node{std::move(x), p->prev, p}};
    }

    // delete the data element pointed by itr; return the iterator pointing to the data element next to the one being deleted
    iterator erase(iterator itr)
    {
        Node* nextNode = itr.current->next;
        iterator retVal{nextNode};
        itr.current->prev->next = nextNode; 
        nextNode->prev = itr.current->prev; 
        delete itr.current;
        theSize--;
        return retVal;
    }

    // delete the data elements from iterator "from" (inclusive) to iterator "to" (exclusive)
    // return the iterator "to"
    iterator erase( iterator from, iterator to )
    { 
        for (auto itr = from; itr != to;)
         itr = erase(itr);
        return to;
    }

    // insert data element x to the very begining of the linked list
    void push_front(const DataType& x)
    { insert(begin(), x); }
  
    void push_front(DataType&& x)
    { insert(begin(), std::move(x)); }
    
    // insert data element x to the very end of the linked list
    void push_back(const DataType& x)
    { insert(end(), x); }
    
    void push_back(DataType&& x)
    { insert(end(), std::move(x)); }
    
    // delete the very first data element
    void pop_front()
    { erase(begin()); }
    
    // delete the very last data element
    void pop_back( )
    { erase(--end()); }
    
    // reverse the entire linked list
    void reverseList()
    {
        for(auto itr = begin(); itr != end(); ++itr) {
         itr = insert(itr, back());
         pop_back();
        }
    }
 
    // append a linked list to the end of the current one
    MyLinkedList<DataType>& appendList(MyLinkedList<DataType>&& rlist) 
    {
        for(auto itr = rlist.begin(); itr != rlist.end(); ++itr)
          push_back(*itr);
        return *this;
    }

    // swap the data element pointed by itr with its next data element
    // return false if the next data element does not exist; true otherwise
    bool swapAdjElements(iterator& itr)
    {
        if(itr.current->next == tail)
          return false;

        std::swap(*itr,itr.current->next->data);
        return true;
    }

};

#endif // __MYLIST_H__
