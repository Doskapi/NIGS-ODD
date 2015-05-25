
//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <map>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <stdint.h>
#include <cmath>

#define TAMANIO_DE_LA_TABLA 50

using namespace std;

struct cuerpoConHash {
    bool sentiment;
    list<unsigned short int> hashTable;
};

uint32_t FNV32(string s);

int hash32(string key, unsigned long int tableSize);

uint64_t FNV64(string s);

int hash64(string key, unsigned long int tableSize);

double productoEscalar(list<unsigned short int> review ,list<double> listaDePesos);

int contarPalabras(string frase);

list <string>  construirNgrama(string, int);

//Retorna true si la palabra es un stopWord, caso contrario retorna false
bool buscarStopWord(string word,list<string> & listaStopWords);

//Incrementa en uno en la posicion indicada;
void incrementar(list<unsigned short int> & hashTable,unsigned long int posicion );

list<double> calcularPesos(map<string, cuerpoConHash> & diccionario, unsigned long int tableSize, int pasosMaximos);

