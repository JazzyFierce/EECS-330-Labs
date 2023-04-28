#ifndef __MYBINARYHEAP_H__
#define __MYBINARYHEAP_H__

#include <iostream>
#include <cstdlib>
#include <math.h>

#include "Lab01_Vector.h"

// ComparableType should be comparable (<, >, ==, >=, <= operators implemented)
// ComparableType should also have +, +=, -, -= operators implemented to support priority adjustment

template <typename ComparableType>
class MyBinaryHeap 
{
  private:
	MyVector<ComparableType> data;  // the array that holds the data elements

    // moves the data element at the pth position of the array up
    void percolateUp(const size_t p) {
        size_t hole = p;
        ComparableType tmp = std::move(data[hole]);

        for (; hole > 1 && tmp > data[hole/2]; hole /=2) {
		//std::cout << data[hole] << " " << data[hole/2] << std::endl;
            data[hole] = std::move(data[hole/2]); 
	}

        data[hole] = std::move(tmp);
 
    }

    // moves the data element at the pth position of the array down
    void percolateDown(const size_t p) {
        size_t hole = p;
        size_t child;
        ComparableType tmp = std::move(data[p]);

        for (; hole*2 <= size(); hole = child) {
            child = hole*2;
            if (child != size() && data[child+1] > data[child])
                ++child;
            if (data[child] > tmp)
                data[hole] = std::move(data[child]);
            else
                break;
        }

        data[hole] = std::move(tmp);
    }

    // reorders the data elements in the array to ensure heap property
    void buildHeap() 
    {
        //std::cout << size() << " " << size()/2 <<std::endl;
        for(int i = size()/2; i > 0; --i)
            percolateDown(i);
    }	

  public: 

    // default constructor
    explicit MyBinaryHeap() :
        data(1)     // reserve data[0]
    {
        ;
    }

    // constructor from a set of data elements
    MyBinaryHeap(const MyVector<ComparableType>& items) : 
        data(1)     // reserve data[0]
    {
        data.reserve(items.size() + 10);
        for (int i = 0; i < items.size(); ++i)
            //data[i+1] = items[i];
            data.push_back(items[i]);
        buildHeap();
    }

    // copy constructor
    MyBinaryHeap(const MyBinaryHeap<ComparableType>& rhs) : data{rhs.data}
    {
        ;
    }

    // move constructor
    MyBinaryHeap(MyBinaryHeap<ComparableType> && rhs) : data{rhs.data}
    {
        rhs.data = nullptr;
    }

    // copy assignment
    MyBinaryHeap& operator=(const MyBinaryHeap<ComparableType>& rhs)   
    {
        MyBinaryHeap copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    // move assignment
    MyBinaryHeap& operator=(MyBinaryHeap<ComparableType> && rhs)
    {
        std::swap(data, rhs.data);
        return *this;
    }

    // inserts x into the priority queue (copy)
	void enqueue(const ComparableType& x)
    {
        /*if (size() == data.size() - 1)
            data.reserve(size()*2);*/

        data.push_back(x);
        percolateUp(data.size()-1);
    } 

    // inserts x into the priority queue (move)
    void enqueue(ComparableType && x)
    {
        /*if (size() == data.size() - 1)
            data.reserve(size()*2);*/

        data.push_back(std::move(x));
        percolateUp(data.size()-1);

    }

    // accesses the data element with the highest priority
	const ComparableType& front() {
        return data[1];
    } 

    // deletes the data element with the highest priority from the queue
    void dequeue() {
        data[1] = std::move(data.back());
        data.pop_back();
        percolateDown(1);
    }

    // verifies whether the array satisfies the heap property
    bool verifyHeapProperty(void)
    {
        for (size_t p = 1; p <= size()/2; p++) {
            if (data[p] < data[(p*2)] || data[p] < data[(p*2)+1]) {
                //std::cout << "Parent: " << data[p] << "; lchild: " << data[p*2] << "; rchild: " << data[(p*2)+1] << std::endl;
                return false;
            }
        }

        return true;
    }

    // disrupts heap property by random shuffling
    void disruptHeapProperty(void) {
        if(data.size() <= 3)
            return;
        for(size_t i = 0; i < 1000; ++ i)
        {
            size_t p = 1 + ((int) rand()) % (data.size() - 1);
            size_t q = 1 + ((int) rand()) % (data.size() - 1);
            std::swap(data[p], data[q]);
        }
        return;
    }

    // merges two heaps; the second heap can be destructed after the merge
    MyBinaryHeap& merge(MyBinaryHeap<ComparableType> && rhs) 
    {
        size_t index = rhs.data.size() -1;
        
        rhs.data[0] = std::move(rhs.data[index]);
        rhs.data.resize(index);
        //std::cout << rhs.data[1] << std::endl;

        data.append(std::move(rhs.data));
        buildHeap();
        //std::cout << front() << std::endl;
        return *this;
    }

    // increases the priority measure of an element at a specific position and reorder the heap
	void increaseKey(const size_t p, const unsigned int d)
    {    
        data[p] += d;
        //std::cout << verifyHeapProperty() << std::endl;
        percolateUp(p);
	    //percolateDown(p);
        //std::cout << verifyHeapProperty() << std::endl;
    }

    // decreases the priority measure of an element at a specific position and reorder the heap
    // if the current priority is smaller than the requested decrement, assign priority 0
	void decreaseKey(const size_t p, const unsigned int d) {
        data[p] = d > data[p] ? 0 : data[p] - d;
        // std::cout<< p << " " << data[p] << std::endl;
        percolateDown(p);
	    //percolateUp(p);
    }

    // checks whether the heap is empty
    bool empty()
    { return data.size() <= 1; }

    // removes all data elements from the heap
    void clear() {
        data.resize(1);       
    }

    // returns the size of the heap
    size_t size() { 
        return data.size()-1; 
    }

    // prints the data in the array
    void print() {
        const char delim = '\t';
        for (size_t i = 1; i < data.size(); ++ i) {
            std::cout << data[i] << delim;
        }
        std::cout << std::endl;
        return;
    }
};

#endif // __MYBINARYHEAP_H__