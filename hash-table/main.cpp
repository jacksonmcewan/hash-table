
#include "main.h"

int main( int argc, char *argv[] )
{
    
    try
    {
        
        Hashtable <string, short, Hasher<string> > ht;

        string word = "jackson";
      
        ht.put(word, 208);
        
        ht.get(word);
                
  
    } // end try block
    
    // CATCH BLOCKS
    
    catch (NoSuchKey &e) // catch block for NoSuchKey
    {
        cout << e.what() << endl;
    }

    catch (DuplicateKey &e) // catch block for DuplicateKey
    {
        cout << e.what() << endl;
    }
    
    catch (...)
    {
        cout << "UNKNOWN ERROR\n";
    }
    
    return 0;
    
} // end main
