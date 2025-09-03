
#ifndef hashtable_h
#define hashtable_h

#include "entry.h"
#include "errors.h"

using namespace std;

template <typename K, typename V, typename H>
class Hashtable {

  public:
    Hashtable( const size_t initCap = 11, const H& hcf = H() );
    Hashtable( const Hashtable<K,V,H> & );
    ~Hashtable();
    size_t capacity() const;
    void clear();
    bool empty() const;
    V &get( const K & ) const; // throws NoSuchKey
    const Hashtable<K,V,H> &operator=( const Hashtable<K,V,H>& );
    V& operator[]( const K & ) const; // throws NoSuchKey
    void put( const K &, const V & ); // throws DuplicateKey
    V remove( const K & ); // throws NoSuchKey
    // replace returns the replaced value
    V replace( const K &, const V & ); // throws NoSuchKey
    size_t size() const;

  private:
    /** the bucket array of this hash table */
    Entry<K,V> **table;
    /** the size of this hash table */
    size_t sz = 0;
    /** the hash-code function of this hash table */
    H hcf;
    /** the maximum load factor (lambda) of this hash table */
    double lambdaMax = 0.75;
    /** minimum load factor for decreasing the table's capacity */
    double lambdaMin = 0.35;
    /** the coefficient a for MAD compression */
    size_t a = 3;
    /** the coefficient b for MAD compression */
    size_t b = 4;
    /** the current index of the capacity in the prime numbers table */
    size_t primeIndex = 0;
    /** the table of roughly doubling prime numbers */
    const vector<size_t> primes = {11, 23, 47, 97, 197, 397, 797, 1597, 3203,
          6421, 12853, 25717, 51437, 102877, 205759, 411527, 823117, 1646237,
          3292489, 6584983, 13169977, 26339969, 52679969, 105359939,
          210719881, 421439783, 842879579, 1685759167, 3371518343,
          6743036717, 13486073473, 26972146961, 53944293929, 107888587883,
          215777175787, 431554351609, 863108703229, 1726217406467,
          3452434812973, 6904869625999, 13809739252051};

    void setA();
    size_t compress( long long ) const;
    void changeCapacity( size_t );
    void rehash( Entry<K,V> **, size_t );

}; // Hashtable<K,V,H> class



template <typename K, typename V, typename H>
Hashtable<K, V, H>::Hashtable( const size_t cpcty, const H& hcf) // default constructor
{

    this->hcf = hcf; // initialize hcf
    
//    srand((unsigned int) time(NULL));
//    a = rand(); // randomizing a
//    b = rand(); // randomizing b
//
    unsigned int i;
    primeIndex = 0;
    
    for (i=0; primes[i] <= cpcty; i++)
    {
        primeIndex = i; // iterate through primes and find capacity stored in primeIndex
    }
    
    table = new Entry<K,V>*[primes[primeIndex]]; // allocating space, creating bucket array of pointers to entry objects (key, value pairs)
        
    for (size_t i = 0; i < primes[primeIndex]; i++)

    {
        table[i] = nullptr; // iterate through table and set pointers to NULL
    }
}



template <typename K, typename V, typename H>
Hashtable<K, V, H>::Hashtable( const Hashtable<K,V,H> & hash) // copy constructor
{
    *this = hash;
}



template <typename K, typename V, typename H>
Hashtable<K, V, H>::~Hashtable() // destructor, calls clear function and deallocates the table
{
    
    clear();
    delete [] table;

}



template <typename K, typename V, typename H>
size_t Hashtable<K, V, H>::capacity() const // capacity function, returns the size (amount of buckets)
{
    return primes[primeIndex];
}



template <typename K, typename V, typename H>
void Hashtable<K, V, H>::clear() // clear function, clears the table
{
    for (unsigned i = 0; i < capacity(); i++)
    {
        Entry<K,V> *current = table[i];
        Entry<K,V> *clearCurrent = table[i]; // second pointer to assist in clearing efforts

        while (current != nullptr)
        {
            clearCurrent = current;
            current = current->nextPtr;
            delete clearCurrent;
            clearCurrent = nullptr;

        } // end while

        table[i] = nullptr;

    }

    sz = 0; // sets size back to zero after clearing
    
}



template <typename K, typename V, typename H>
bool Hashtable<K, V, H>::empty() const // empty function, checks if empty
{
    if (sz == 0)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}



template <typename K, typename V, typename H>
V& Hashtable<K, V, H>::get( const K & getKey ) const // get function, finds value for given key
{
    long long loc = compress(hcf(getKey));
    Entry<K,V> *current = table[loc];
            
    if (table[loc] == nullptr)
    {
        throw NoSuchKey("ERROR: key not found!\n");
    }
            
    else
    {

        while (current != nullptr) // while table[loc] is not empty
        {

            if (current->key == getKey)
            {
                return current->value;
                
            } // end if
                
            current = current->nextPtr; // iterates, restarts the while loop

        } // end while
        
        throw NoSuchKey("ERROR: key not found after iterating in get()"); // if this is reached, throws error
                
    } // end else
        
    return current->value;

}

    

template <typename K, typename V, typename H>
const Hashtable<K,V,H>& Hashtable<K,V,H>::operator=(const Hashtable<K,V,H> & copyHCF) // overloaded assignment operator
{
        
    this->clear();
    
    Entry<K,V> **newTable;
    newTable = new Entry<K,V>*[primes[primeIndex]];

    for (size_t i = 0; i < primes[primeIndex]; i++)
    {
        newTable[i] = nullptr;
    }

    Entry<K,V> *current = nullptr;

    for (unsigned i = 0; i < capacity(); i++)
    {
        
        current = table[i];

        if (copyHCF.table[i] == nullptr)
        {
            newTable[i] = nullptr;
        }

        else
        {
            Entry<K,V> *toPointer = copyHCF.table[i];

            while (toPointer != nullptr)
            {
                
                Entry<K,V> *newEntry = new Entry<K,V>(toPointer->key, toPointer->value, toPointer->h);
                
                if (table[i] == nullptr)
                {
                    table[i] = newEntry;
                    newEntry->nextPtr = nullptr;
                }
                
                else
                {
                    current = table[i];
                    
                    current->prevPtr = newEntry;
                    newEntry->nextPtr = current;

                    table[i] = newEntry;
                    
                }
                
                toPointer = toPointer->nextPtr;
                
                
            } // end while

        } // end else

    } // end for
    
    return *this;
    
}



template <typename K, typename V, typename H>
V& Hashtable<K, V, H>::operator[]( const K & entry) const // overloaded bracket operator (calls get() function)
{
    return get(entry);
}



template <typename K, typename V, typename H>
void Hashtable<K, V, H>::put( const K & putKey, const V & putValue) // put function, inserts key, value pair
{
    
    long long loc = compress(hcf(putKey));
                        
    Entry<K,V> *newEntry = new Entry<K,V>(putKey, putValue, loc);

    if (table[loc] == nullptr)
    {
        table[loc] = newEntry;
    }

    else
    {
                
        Entry<K,V> *putCurrent = table[loc];
        
        while (putCurrent != nullptr)
        {
            
            if (putCurrent->key == putKey)
            {
                throw DuplicateKey("ERROR: duplicate key!\n"); // catches error for key already present
            }
            
            else
            {
                putCurrent = putCurrent->nextPtr;
            }
            
        } // end while
        
        putCurrent = table[loc];
                        
        table[loc] = newEntry;
        
        putCurrent->prevPtr = newEntry;
        newEntry->nextPtr = putCurrent;

    }

    sz++;
    
    double loadFactor = (sz * 1.0)/primes[primeIndex]; // * 1.0 allows for comparison between types size_t and double
    
    if (loadFactor > lambdaMax)
    {
        changeCapacity(primeIndex); // calls changeCapacity with given primeIndex
    }

    if (loadFactor < lambdaMin && primeIndex > 1)
    {
        changeCapacity(primeIndex); // calls changeCapacity with given primeIndex
    }
    
    
    
}
        
    
    
template <typename K, typename V, typename H>
V Hashtable<K, V, H>::remove( const K & removeKey) // remove function, removes key, value pair of given key
// MAKE SURE TO IMPLEMEMT EMPTY V AVAILABLE
{
    
    V val = 0;
        
    long long loc = compress(hcf(removeKey));

    Entry<K,V> *temp = table[loc]; // node to delete

    if (temp == nullptr) // if the bucket is empty
    {
        throw NoSuchKey("ERROR: key not found in remove() function!\n");

    }
        
    if (temp->nextPtr == nullptr) // if the bucket only has one element
    {

        if (temp->key == removeKey) // if it is the one we want to delete
        {
            val = temp->value;
                
            delete temp;

            table[loc] = nullptr;
            sz--;

            return val;
                
        } // end if
        
        else
        {
            throw NoSuchKey("ERROR: key not found!");
        }

    } // end if
    
    else // if the bucket has more than one element
    {
        temp = table[loc];
        
        if (temp->key == removeKey) // if its the first of many
        {
            
            table[loc] = temp->nextPtr;
            temp->nextPtr->prevPtr = nullptr;
            delete temp;
            sz--;
            
            return val;
        }
        
        Entry<K,V> *lastElement = table[loc];
        
        while (lastElement->nextPtr != nullptr)
        {
            lastElement = lastElement->nextPtr; // lastElement now points to the last element
        }
        
        if (lastElement->key == removeKey) // its the last element
        {
            lastElement->prevPtr->nextPtr = nullptr;
            delete lastElement;
            sz--;
            
            return val;
        }
        
        temp = table[loc];
        temp = temp->nextPtr;
        
        while (temp->nextPtr != nullptr) // its in the middle
        {
            
            if (temp->key == removeKey)
            {
                temp->nextPtr->prevPtr = temp->prevPtr;
                temp->prevPtr->nextPtr = temp->nextPtr;
                delete temp;
                sz--;
                
                return val;
            }
          
            temp = temp->nextPtr;
        }
            
        throw NoSuchKey("\nERROR: key not found after iterating in remove() function!\n"); // not found!
            
    } // end original else
        
    return val;
    
}



template <typename K, typename V, typename H>
V Hashtable<K, V, H>::replace( const K & replaceKey, const V & replaceValue) // replace function, replaces given key's value with a given value
{
    long long loc = compress(hcf(replaceKey));
    Entry<K,V> *replaceCurrent = table[loc];
        
    if (table[loc] == nullptr)
    {
        throw NoSuchKey("ERROR: key not found!\n"); // catches error for keys not found on linked list
    }
        
    else
    {

        while (replaceCurrent != nullptr)
        {
            if (replaceCurrent->key == replaceKey)
            {
                replaceCurrent->value = replaceValue;
                return replaceCurrent->value;
            }
            
            replaceCurrent = replaceCurrent->nextPtr;

        } // end while
                
    } // end else
    
    return replaceCurrent->value;
}



template <typename K, typename V, typename H>
size_t Hashtable<K, V, H>::size() const // size function, returns quantity of elements in table
{
    return sz;
    
}



template <typename K, typename V, typename H>
void Hashtable<K, V, H>::setA() // setA function, sets a to avoid instance where (a % N = 0)
{
    a++;
}



template <typename K, typename V, typename H>
size_t Hashtable<K, V, H>::compress( long long hash) const // compress function, converts hcf results to placeable integer within capacity
{
    long long loc = 0;
    
    long long value = ((a * hash) + b); // value to calculate absolute value
    size_t cpcty = capacity(); // capacity to use as N
    
    loc = abs(value) % cpcty; // polynomial compression function
    
    return loc;

}



template <typename K, typename V, typename H>
void Hashtable<K, V, H>::changeCapacity( size_t cpcty ) // changeCapacity function, changes capacity of the table (quantity of buckets)
{
        
    Entry<K,V> **oldTable = table;
        
    double lambda = sz*1.0 / capacity();
                
    srand((unsigned int) time(NULL));

    a = rand();
        
    if (a % capacity() == 0)
    {
        setA(); // increments a if the randonmly-generated a conflicts with the capacity
    }
    
    size_t oldIndex = primeIndex;
    
    if (lambda > lambdaMax)
    {
        primeIndex++; // increments the size to be passed into the newTable
    }

    if (lambda < lambdaMin && primeIndex >= 1)
    {
        primeIndex--; // decrements the size to be passed into the newTable
    }
    
    rehash(oldTable, oldIndex);
}



template <typename K, typename V, typename H>
void Hashtable<K, V, H>::rehash( Entry<K,V> ** oldTable, size_t oldIndex ) // rehash function, copies table to new table with roughly double capacity (if called)
{
    
    Entry<K,V> **newTable;
    newTable = new Entry<K,V>*[primes[primeIndex]];
        
    for (unsigned i = 0; i < primes[primeIndex]; i++)
    {
        newTable[i] = nullptr;
    }
                
    Entry<K,V> *oldCurrent = nullptr;
    Entry<K,V> *newCurrent = nullptr;

    for (size_t i = 0; i < primes[oldIndex]; i++)
    {
        
        oldCurrent = oldTable[i];
        newCurrent = newTable[i];

        while (oldCurrent != nullptr)
        {
            long long compressed = compress(oldCurrent->h);
            
            if (newTable[compressed] == nullptr)
            {
                                
                newTable[compressed] = oldCurrent;
                oldCurrent->nextPtr = nullptr;
                
            }
                        
            else if (newTable[compressed] != nullptr) // this is where it goes wrong/breaks
            {
                newCurrent = newTable[compressed];
                
                while (newCurrent->nextPtr != nullptr)
                {
                    newCurrent = newCurrent->nextPtr;
                }
                
                newCurrent->nextPtr = oldCurrent;
                oldCurrent->nextPtr = nullptr;
            
            }
            
            oldCurrent = oldCurrent->nextPtr;
            
        }

    }
        
    delete [] oldTable;
    table = newTable;
}



#endif /* hashtable_h */
