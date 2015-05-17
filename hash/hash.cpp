#include "hash.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;


Hash::Hash()
{
    tableSize = 100;
}

int Hash::hash(string key)
{
    int index,hash = 0;

    //sum the ascci value of each letter in the key
    for (int i = 0; i < key.length(); i++)
    {
        hash += (int)key[i];
        cout << "hash leter " << key[i] << "=" << (int)key[i] << " sum: " << hash << endl;         
    }
         
    index = hash%tableSize;

    return index;
}

Hash::~Hash()
{
    //dtor
}
