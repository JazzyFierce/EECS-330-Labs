#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include <iostream>
#include <algorithm>

#include "Lab01_Vector.h"

template <typename DataType>
class MyStack : private MyVector<DataType>
{  
  public:

    // default constructor
    explicit MyStack(size_t initSize = 0) { ; }

    // copy constructor
    //MyStack(const MyStack & rhs) : MyVector(rhs) {;}
    MyStack(const MyStack&) = default;

    // move constructor
    //MyStack(MyStack && rhs) : MyVector(std::move(rhs)) {;}
    MyStack(MyStack&&) = default;


    // destructor
    ~MyStack() {;}

    // copy assignment
    MyStack & operator= (const MyStack & rhs)
    {
        MyVector::operator=(rhs);
        return *this
    }

    // move assignment
    MyStack & operator= (MyStack && rhs)
    {
        MyVector::operator=(std::move(rhs));
        return *this;
    }

    // insert x to the stack
    void push(const DataType & x) { this->push_back(x); }

    // insert x to the stack
    void push(DataType && x) { this->push_back(std::move(x)); }

    // remove the last element from the stack
    void pop(void) { this->pop_back(); }

    // access the last element of the stack
    const DataType & top(void) const { return this->back(); }

    // check if the stack is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const { return this->size() == 0; }

    // access the size of the stack
    size_t size() const { return this->theSize(); }

    // access the capacity of the stack
    size_t capacity(void) const { return this->theCapacity(); }

};


#endif // __MYSTACK_H__