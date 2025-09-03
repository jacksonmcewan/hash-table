
#ifndef entry_h
#define entry_h

template <typename K, typename V>
class Entry
{

    public:
    K key;
    V value;
    

    long long h; // this is where the hash code of the key is stored
                 // makes rehashing and dynamic allocation more efficient
    
    Entry<K,V> *nextPtr;
    Entry<K,V> *prevPtr;
    
    Entry(const K&, const V&, long long h);


};

template <typename K, typename V>
Entry<K,V>::Entry(const K& newKey, const V& newValue, long long newHash)
{
    key = newKey;
    value = newValue;
    h = newHash;
    
    nextPtr = nullptr;
    prevPtr = nullptr;
}

#endif /* entry_h */


