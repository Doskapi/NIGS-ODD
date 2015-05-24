
//Inclusiones necesarias
#include "funciones.h"

//Constantes para las funciones hash
#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U
#define FNV_PRIME_64 1099511628211
#define FNV_OFFSET_64 14695981039346656037U

//utiliza FNV32 para hashear un string y normaliza a la tabla de hash
int hash32(string key, unsigned long int tableSize) {
    int index;
    index = FNV32(key);
    index = index%tableSize;
    return index;
}

uint32_t FNV32(string s) {
    // Hash FNV 32
    int a = s.size();
    uint32_t hash = FNV_OFFSET_32;
    for(int i = 0; i < a; i++)
    {
        hash = hash ^ (s[i]);
        hash = hash * FNV_PRIME_32;
    }
    return hash;
}

//utiliza FNV64 para hashear un string y normaliza a la tabla de hash
int hash64(string key, unsigned long int tableSize) {
    int index;
    index = FNV64(key);
    index = index%tableSize;
    return index;
}

uint64_t FNV64(string s) {
    // Hash FNV 64
    int a = s.size();
    uint64_t hash = FNV_OFFSET_64;
    for(int i = 0; i < a; i++) {
        hash = hash ^ (s[i]);
        hash = hash * FNV_PRIME_64;
    }
    return hash;
}

int contarPalabras(string frase) {
    int cantidad = 0;
    stringstream ss(frase);
    string s;
    while (getline(ss, s, ' ')) {
        cantidad ++;
    }
    return cantidad;
}

//Dado una frase cualquiera y el tamanio del n grama devuleve
list <string>  construirNgrama(string frase, int tamanioNgrama) {
    list <string> listaNgramas;
    int cantidadPalabras = contarPalabras(frase);
    if (cantidadPalabras > tamanioNgrama) {
        stringstream unStream(frase);
        string palabra1, palabra2, unNgrama;
        int contadorNgrama = 0;
        while (getline(unStream, palabra1, ' ')) {
            unNgrama = "";
            palabra2 = palabra1;
            stringstream streamSegundario(frase);
            for(int i= 0; i< contadorNgrama; i++){
                getline(streamSegundario, palabra2, ' ');
            }
            for(int i= 0; i< tamanioNgrama; i++){
                getline(streamSegundario, palabra2, ' ');
                if (i < tamanioNgrama) unNgrama += palabra2 + " ";
                else unNgrama += palabra2;
            }
            if ((contadorNgrama + tamanioNgrama ) <= cantidadPalabras){
                    listaNgramas.push_back(unNgrama);
            }
            contadorNgrama++;
        }
    } else listaNgramas.push_front(frase);
	return listaNgramas;
}

/* Retorna true si la palabra es un stopWord, caso contrario retorna false */
bool buscarStopWord(string word, list<string> & listaStopWords ) {
    list<string>::iterator iterador;
    for (iterador = listaStopWords.begin(); iterador != listaStopWords.end(); iterador++ ){
        if (*iterador == word) return true;
    }
    return false;
}

void incrementar(list<unsigned short int> & hashTable,unsigned long int posicion ) {
    unsigned long int k = 0;
    list<unsigned short int>::iterator iter;
    for (iter = hashTable.begin();(k < posicion); iter++ ) k++;
    *iter = *iter + 1;
}

