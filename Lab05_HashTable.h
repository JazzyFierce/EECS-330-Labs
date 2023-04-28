#ifndef __MYHASHTABLE_H__
#define __MYHASHTABLE_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>

#include "Lab01_Vector.h"
#include "Lab02_LinkedList.h"

static const long long uh_param_a = 53;       // universal hash function parameter a
static const long long uh_param_b = 97;       // universal hash function parameter b
static const long long prime_digits = 19;     // parameter used for finding a Mersenne prime
static const long long mersenne_prime = (1 << prime_digits) - 1;  // the Mersenne prime for universal hashing

// fast calculation of (n modulo mersenne_prime)
long long fastMersenneModulo(const long long n)
{
    long long hv = (n>>prime_digits)+(n & mersenne_prime);

    if (hv >= mersenne_prime)
        hv -= mersenne_prime;
    
    return n;
}

// definition of the template hash function class (PREIMPLEMENTED: DO NOT TOUCH)
template <typename KeyType>
class HashFunc
{
  public:
    long long univHash(const KeyType key, const long long table_size) const;
};

// the hash function class that supports the hashing of the "long long" data type (PREIMPLEMENTED: DO NOT TOUCH)
template <>
class HashFunc<long long>
{
  public:
    long long univHash(const long long key, const long long table_size) const
    {
        long long hv = fastMersenneModulo(static_cast<long long>(uh_param_a * key + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// the has function class that supports the hashing of the "std::string" data type (PREIMPLEMENTED: DO NOT TOUCH)
template <>
class HashFunc<std::string>
{
  private:
    const int param_base = 37;      // the base used for inflating each character
  public:    
    long long univHash(const std::string& key, const long long table_size) const
    {
        long long hv = 0;
        for(size_t i = 0; i < key.length(); ++ i)
        {
            hv = param_base * hv + static_cast<long long>(key[i]);
        }
        hv = fastMersenneModulo(static_cast<long long>(uh_param_a * hv + uh_param_b));

	    std::cout << "hv before mode " << hv << std::endl;
        hv = hv % table_size;
	    std::cout << "hv after mode " << hv << std::endl;

        return hv;
    }
};

// definition of the template hashed object class (PREIMPLEMENTED: DO NOT TOUCH)
template <typename KeyType, typename ValueType>
class HashedObj
{
  public:

    KeyType key;
    ValueType value;

    HashedObj()
    {
        return;
    }

    HashedObj(const KeyType& k, const ValueType& v) :
        key(k),
        value(v)
    {
        return;
    }
  
    HashedObj(KeyType && k, ValueType && v) :
        key(std::move(k)),
        value(std::move(v))
    {
        return;
    }

    bool operator==(const HashedObj<KeyType, ValueType>& rhs)
    {
        return (key == rhs.key);
    }    

    bool operator!=(const HashedObj<KeyType, ValueType>& rhs)
    {
        return !(*this == rhs);
    }   

};

// MY CODE BEGINS
template <typename KeyType, typename ValueType>
class MyHashTable
{ 
  private:
    size_t theSize; // the number of data elements stored in the hash table
    MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > hash_table;    // the hash table implementing the separate chaining approach
    MyVector<size_t> primes;    // a set of precomputed and sorted prime numbers

    // pre-calculate a set of primes using the sieve of Eratosthenes algorithm
    // will be called if table doubling requires a larger prime number for table size
    // expected to update the private member "primes"
    void preCalPrimes(const size_t n)
    {
        // code and comments from https://www.geeksforgeeks.org/sieve-of-eratosthenes/

        // Create a boolean array "prime[0..n]" and initialize
        // all entries it as true. A value in prime[i] will
        // finally be false if i is Not a prime, else true.
        bool prime[n + 1];
        memset(prime, true, sizeof(prime));
        // look at me micro-optimizing. I'll probably have to delete this late because things never go to plan
        size_t cur_max_prime_index_buffer = primes.size() > 0 ? primes.back()+1 : 0;

        for (int p = 2; p * p <= n; p++) {
            // If prime[p] is not changed, then it is a prime
            if (prime[p + cur_max_prime_index_buffer] == true) {
                // Update all multiples of p greater than or
                // equal to the square of it numbers which are
                // multiple of p and are less than p^2 are
                // already been marked.
                for (int i = p * p; i <= n; i += p)
                    prime[i] = false;
            }
        }

        int start = primes.size() > 0 ? primes.back() + 1 : 2;
        // update primes, starting at next biggest prime
        for (int p = start; p <= n; p++)
            if (prime[p])
                primes.push_back(prime[p]);
    }

    // finding the smallest prime that is larger than or equal to n
    // should perform binary search against the private member "primes"
    size_t nextPrime(const size_t n)
    {
        // if no primes are >= n, get more primes
        while (n > primes.back())
            preCalPrimes(primes.size()*2);

        int lo = 0;
        int hi = primes.size() - 1;
        size_t mid;

        while (hi - lo > 1) {
            mid = ceil((hi + lo) / 2);
            if (primes[mid] == n)
                return primes[mid];


            if (primes[mid] < n) {
                lo = mid + 1;
            }
            else {
                hi = mid;
            }
        }

        if (primes[lo] >= n) {
            return primes[lo];
        } else {
            return primes[lo+1];
        }
    }

    // finds the MyLinkedList itertor that corresponds to the hashed object that has the specified key
    // returns the end() iterator if not found
    typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator find(const KeyType& key)
    {
	std::cout << this->capacity() << std::endl;
	std::cout << "find(key) start" << std::endl;
        HashFunc<KeyType> hasher = HashFunc<KeyType>();
	
        MyLinkedList<HashedObj<KeyType, ValueType> >* whichList = hash_table[hasher.univHash(key, capacity())];
        std::cout << "whichList found" << std::endl;
	
        for (typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator itr = whichList->begin(); itr != whichList->end(); itr++) {
            if ((*itr).key == key)
                return itr;
        }

        return whichList->end();
    }

    // rehashes all data elements in the hash table into a new hash table with new_size
    // note that the new_size can be either smaller or larger than the existing size
    void rehash(const size_t new_size)
    {
        // copy old entries to rehash in a bit
        MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* >  oldLists = hash_table;

        // resize, either smaller or bigger
        hash_table.resize(new_size);

        // clear hash table
        for (auto& thisList : hash_table)
            thisList->clear();

        // copy over old entries
        theSize = 0;
        for (typename MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* >::iterator thisList = oldLists.begin(); thisList != oldLists.end(); thisList++) {
            auto bucket = *thisList;
            for (typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator itr = bucket->begin(); itr != bucket->end(); bucket++) {
                insert(std::move(*itr));
            }
        }


    }

    // doubles the size of the table and perform rehashing
    // the new table size should be the smallest prime that is larger than the expected new table size (double of the old size)
    void doubleTable()
    {
        size_t new_size = nextPrime(2 * hash_table.size());
        this->rehash(new_size);
        return;
    }

    // halves the size of the table and perform rehahsing
    // the new table size should be the smallest prime that is larger than the expected new table size (half of the old size)
    void halveTable()
    {
        size_t new_size = nextPrime(ceil(hash_table.size() / 2));
        this->rehash(new_size);
        return;
    }

  public:

    // the default constructor; allocate memory if necessary
    explicit MyHashTable(const size_t init_size = 3)
    {
	for (int i = 0; i < init_size; i++) {
		MyLinkedList<HashedObj<KeyType, ValueType> >* newList = new MyLinkedList<HashedObj<KeyType, ValueType> >();
		hash_table.push_back(newList);
	}
		
	std::cout << hash_table.size() << std::endl;
	std::cout << hash_table.capacity() << std::endl;
	theSize = 0;
        preCalPrimes(100000);
    }

    // the default destructor; collect memory if necessary
    ~MyHashTable()
    {
        for (auto& list : hash_table) {
            list->clear();
            delete list; // is this necessary?
        }
            
    }

    // checks if the hash tabel contains the given key
    bool contains(const KeyType& key)
    {
        HashFunc<KeyType> hasher = HashFunc<KeyType>();
        auto& whichList = hash_table[hasher.univHash(key, theSize)];
        return find(key) != whichList->end();
    }

    // retrieves the data element that has the specified key
    // returns true if the key is contained in the hash table
    // return false otherwise
    bool retrieve(const KeyType& key, HashedObj<KeyType, ValueType>& data)
    {
        return contains(key);
    }

    // inserts the given data element into the hash table (copy)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(const HashedObj<KeyType, ValueType>& x)
    {
	    std::cout << "begin insert" << std::endl;
        HashFunc<KeyType> hasher = HashFunc<KeyType>();
        auto& whichList = hash_table[hasher.univHash(x.key, capacity())];
	    std::cout << "List found" << std::endl;

	    find(x.key);

        // if the key is already contained in the table, end the method

        if (find(x.key) != whichList->end())
            return false;
	std::cout << "Entry not in table" << std::endl;
        whichList->push_back(x);

        if (++theSize > hash_table.size())
            doubleTable();

        return true;
    }

    // inserts the given data element into the hash table (move)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(HashedObj<KeyType, ValueType> && x)
    {
        HashFunc<KeyType> hasher = HashFunc<KeyType>();
        auto& whichList = hash_table[hasher.univHash(x.key, capacity())];
        // if the key is already contained in the table, end the method
        if (find(x.key) != whichList->end())
            return false;
        whichList->push_back(std::move(x));

        if (++theSize > hash_table.size()/2)
            doubleTable();

        return true;
    }

    // removes the data element that has the key from the hash table
    // returns true if the key is contained in the hash table
    // returns false otherwise
    bool remove(const KeyType& key)
    {
        HashFunc<KeyType> hasher = HashFunc<KeyType>();
        auto& whichList = hash_table[hasher.univHash(key, theSize)];
        auto itr = find(key);

        // if the element is not contained in the table, end
        if (itr == whichList->end())
            return false;

        whichList->erase(itr);

        if (--theSize < hash_table.size()/8)
            halveTable();
        return true;   
    }

    // returns the number of data elements stored in the hash table; i.e., N
    size_t size() { return theSize; }

    // returns the capacity of the hash table; i.e., M
    size_t capacity() { return hash_table.capacity(); }
};


#endif // __MYHASHTABLE_H__
