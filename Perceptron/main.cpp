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
    unsigned long int tableSize = 20;//33554432; //deberia ser 2^25 = 33554432
    //unsigned long int hashTable[tableSize];
    list<unsigned int> hashTable (tableSize);
    list<unsigned int>::iterator iter; //Esta es la forma de recorrer esta lista
    for (iter = hashTable.begin(); iter != hashTable.end(); iter++ ){
        *iter = 0;
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
        incrementar(hashTable, returnValue32);
    }

    cout << "[pos]:hashvalue" << endl;
    int j = 0;
    for (iter = hashTable.begin(); iter != hashTable.end(); iter++ )
    {
        cout << "["<< j << "]:" << *iter << "  " << endl;
        if ((j%9 == 0) && (j>1))
        {
            cout << endl;
        }
    }
    cout << endl;
}

int main() {
    pruebas();
    return 0;
}
