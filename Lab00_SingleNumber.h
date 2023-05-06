#ifndef __MYNUMBER_H__
#define __MYNUMBER_H__

#include <algorithm>
#include <iostream>
using namespace std;

template <typename DataType>
class MyNumber
{
  public:
    explicit MyNumber(DataType rhs = 0) // default constructor
    { num = new DataType { rhs }; }
    
    MyNumber(const MyNumber<DataType>& rhs)	// copy constructor with a MyNumber instance
    { num = new DataType{ *rhs.num }; }
    
    MyNumber(MyNumber<DataType> && rhs)	: num{ rhs.num } // move constructor with a MyNumber instance
    { rhs.num = nullptr; }
    
    MyNumber & operator= (const MyNumber<DataType> &rhs)	// copy assignment with a MyNumber instance
    {
        if (this != &rhs)
          *num = *rhs.num;
        return *this;
    }

    MyNumber & operator= (MyNumber<DataType> && rhs)	// move assignment with a MyNumber instance 
    {
        swap(num, rhs.num);
        return *this;
    }

    ~MyNumber(void) // destructor
    { delete num; }
    
    DataType read(void) const
    { return *num; }

    void write(DataType rhs)
    { *num = rhs; }

  private:
    /* data */
    DataType *num;
};


#endif 
