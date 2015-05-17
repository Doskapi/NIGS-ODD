
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
        int hashPearson(string key);
        int getTableSize();
        void setTableSize(int tSize);
        void addItem(string value);
        void printTable();
    protected:
    private:
        static const int tableSize = 10;
        struct item
        {
            int value;
        };
        item* hashTable[tableSize];
};

#endif // HASH_H
