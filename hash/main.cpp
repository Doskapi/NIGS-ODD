/************************************************************************************
 * HASH Function
 * Facultad de Ingeniería - Universidad de Buenos Aires
 * 75.06 - Organizacion de Datos
 * Cátedra Argerich
 * Tp Grupal - Grupo 1 - Araya Campoamor Goicoa Recalde
 *************************************************************************************/
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

#include "hash.h"

using namespace std;

int main(){
    
    Hash myHash;
    string stringToHash;
    int returnValue;

    cout << endl;
    cout << "Prueba de Hash" << endl;
    cout << "Inserte string a hashear: " << endl;
    cin >> stringToHash;
    cout << endl;

    returnValue = myHash.hash(stringToHash);

    cout << "el valor del hash \"" << stringToHash << "\" es: " << returnValue << endl;

    return 0;
}
