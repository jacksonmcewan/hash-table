
#ifndef hasher_h
#define hasher_h

template<typename K> // HASHER CLASS DECLARATION
class Hasher
{
    public:
    long long operator()( const K & ) const; // needs to work as if it were a function
};


template <typename K> // OVERLOADED PARENTHESES () OPERATOR
long long Hasher<K>::operator()( const K &key) const
{
    int g = 31;
    long long hash = 0;
    
    for (long unsigned int i = 0; i < key.length(); i++)
    {
        hash = (g * hash) + key.at(i);
    }
    
    return hash; // uncompressed integer to be entered into compressor function
    
} // operator()


#endif /* hasher_h */
