
#ifndef errors_h
#define errors_h

using namespace std;

class NoSuchKey : public logic_error
{
public:
    NoSuchKey(string = "ERROR: key not found!");
};

class DuplicateKey : public logic_error
{
public:
    DuplicateKey(string = "ERROR: duplicate key!");
};

NoSuchKey::NoSuchKey(string msg) : logic_error (msg)
{
    
}

DuplicateKey::DuplicateKey(string msg) : logic_error (msg)
{
    
}

#endif /* errors_h */
