#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <iostream>
#include <algorithm>

#include "Lab01_Vector.h"

template <typename DataType>
class MyQueue : private MyVector<DataType>
{  
  private:
    size_t dataStart, dataEnd;

    // changes the size of the array to newSize
    void resize(size_t newSize) { MyVector<DataType>::resize(newSize); }

    // requests for newCapacity amount of space
    void reserve(size_t newCapacity)
    {
        if (newCapacity > capacity()) {
            int old_cap = capacity();

            MyVector<DataType>::reserve(newCapacity);
            // c d e a b --> c d e a b (r a n d o)
            if (dataStart > dataEnd) {
                typename MyVector<DataType>::const_iterator first_empty = MyVector<DataType>::begin();
                int counter = 0;
                while (counter != old_cap) {
                    first_empty++;
                    counter++;
                }

                dataEnd = old_cap - 1;
                typename MyVector<DataType>::const_iterator resumed_data = MyVector<DataType>::begin();

                for (size_t i = 0; i != dataStart; i++) {
                    std::swap(*first_empty, *resumed_data);
                    dataEnd++;
                }
            }   
        }
    }

  public:

    // default constructor
    explicit MyQueue(size_t initSize = 0) : MyVector<DataType>(), dataStart{1}, dataEnd{0} { ; }

    // copy constructor
    MyQueue(const MyQueue & rhs) : MyVector<DataType>(rhs), dataStart{rhs.dataStart}, dataEnd{rhs.dataEnd} { ; }

    // move constructor
    MyQueue(MyQueue && rhs) : MyVector<DataType>(std::move(rhs))
    {
	    dataEnd = rhs.dataEnd;
	    dataStart = rhs.dataStart;
    }

    // destructor
    ~MyQueue() { ; }

    // copy assignment
    MyQueue & operator= (const MyQueue & rhs)
    {
	    MyQueue copy = rhs;
	    std::swap(*this, copy);
	    return *this;	
    }

    // move assignment
    MyQueue & operator= (MyQueue && rhs)
    {
	    MyVector<DataType>::operator=(std::move(rhs));
	    std::swap(dataStart, rhs.dataStart);
	    std::swap(dataEnd, rhs.dataEnd);
	    return *this;
    }

    // insert x into the queue
    void enqueue(const DataType& x)
    {
        typename MyVector<DataType>::const_iterator itr = MyVector<DataType>::begin();
        for (size_t i = 0; i != dataEnd + 1; i++) 
            itr++;
        std::swap(*itr, x);
        dataEnd = (dataEnd+1)%(this->capacity());
        resize(MyVector<DataType>::size()+1)
    }

    // insert x into the queue
    void enqueue(DataType && x)
    {
        typename MyVector<DataType>::iterator itr = MyVector<DataType>::begin();

        for (size_t i = 0; i != dataEnd + 1; i++) 
            itr++;

	    std::swap(*itr, x);
        dataEnd = (dataEnd+1)%(this->capacity());
        resize(MyVector<DataType>::size()+1)
    }

    // remove the first element from the queue
    void dequeue(void) { 
        dataStart = (dataStart+1)%(this->capacity());
        resize(MyVector<DataType>::size()-1)
    }

    // access the first element of the queue
    const DataType & front(void) const
    {
	    typename MyVector<DataType>::const_iterator itr = MyVector<DataType>::begin();
	    for (size_t i = 0; i != dataStart; ++i)
		    ++itr;
	    return *itr;
    }

    // check if the queue is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const { return this->size()==0; }

    // access the size of the queue
    size_t size() const { return MyVector<DataType>::size() }

    // access the capacity of the queue
    size_t capacity(void) const { return MyVector<DataType>::capacity(); }

};

#endif // __MYQUEUE_H__