//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include "funciones.h"
#include "archivos.h"

using namespace std;

void pruebas(){
    string frase = "una frase loca para mostrar que este n grama si funciona correctamente saludos";
    list<string> listaNgramas = construirNgrama(frase, 3);
    list<string>::iterator iterador; //Esta es la forma de recorrer esta lista
    int i = 0;
    for (iterador = listaNgramas.begin(); iterador != listaNgramas.end(); iterador++ ){
        cout << i << ") " ;
        cout << *iterador << endl;
        i++;
    }
    list<string> listaStopWords = crearListaDeStopWords();
    i = 0;
    for (iterador = listaStopWords.begin(); iterador != listaStopWords.end(); iterador++ ){
        cout << i << ") " ;
        cout << *iterador << endl;
        i++;
    }

    cout << buscarStopWord("to",listaStopWords) << endl;

    string stringToHash;
    uint32_t returnValue32;
    unsigned long int tableSize = 1000; //deberia ser 2^25 = 33554432
    unsigned long int hashTable[tableSize];

    for (int i = 0; i < (int)tableSize; i++)
    {
        hashTable[i] = 0;
    }

    for (int i = 0; i < 3; ++i)
    {
        cout << endl;
        cout << "Prueba de Hash" << endl;
        cout << "=============================================" << endl;
        cout << "Inserte string a hashear: ";
        cin >> stringToHash;
        cout << endl;

        returnValue32 = hash32(stringToHash, tableSize);
        cout << "Hash32: " << returnValue32 << endl;
        // aca deberia poner el dato que se almacena en el hash
        hashTable[returnValue32] = tableSize;
    }

    cout << "[pos]:hashvalue" << endl;

    for (int j = 0; j < (int)tableSize; j++)
    {
        cout << "["<< j << "]:" << hashTable[j] << "  ";
        if ((j%9 == 0) && (j>1))
        {
            cout << endl;
        }
    }
    cout << endl;
}

int main() {
    //pruebas();
    return 0;
}
