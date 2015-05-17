
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>


using namespace std;

#ifndef HASH_H
#define HASH_H


class Hash
{
    public:
        Hash();
        virtual ~Hash();
        int hash(string key);
    protected:
    private:
        int tableSize;
};

#endif // HASH_H
