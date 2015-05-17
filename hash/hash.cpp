#include "hash.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>


using namespace std;


Hash::Hash()
{
    for (int i = 0; i < tableSize; i++)
    {
        hashTable[i] = new item;
        hashTable[i]->value = 0;
    }
}

//void Hash::setTableSize(int tSize)
//{
//    tableSize = tSize;
//}

int Hash::getTableSize()
{
    return tableSize;
}

void Hash::addItem(string value)
{
    int index = hash(value);
    if (hashTable[index]->value == 0)
    {
        hashTable[index]->value = 1;
    }
    else
    {
        hashTable[index]->value += 1;
    }
}

void Hash::printTable()
{
    cout << "pos  hashvalue" << endl;
    for (int i = 0; i < tableSize; i++)
    {
        cout << i << "      " << hashTable[i]->value << endl;
    }
}

int Hash::hash(string key)
{
    int index,hash = 0;

    //sum the ascci value of each letter in the key
    for (int i = 0; i < key.length(); i++)
    {
        hash += (int)key[i];
    }

    index = hash%tableSize;

    return index;
}

int Hash::hashPearson(string key)
{
    int index,hash = 0;

    for (int i = 0; i < key.length(); i++)
    {
        index = hash xor ((int)key[i]);
        cout << "hash leter " << key[i] << "=" << (int)key[i] << " sum: " << hash << endl;
    }

    index = hash%tableSize;

    return index;
}

Hash::~Hash()
{
    /*dstr*/
}



