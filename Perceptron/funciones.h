
//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <map>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <map>
#include <string>
#include <cmath>

using namespace std;

struct cuerpoDelReview {
    bool sentiment;
    string review;
};

struct cuerpoConNgramas {
    bool sentiment;
    list<string> listaReview;
};

struct cuerpoConHash {
    bool sentiment;
    list<char> hashTable;
};

uint32_t FNV32(string & s);

int hash32(string & key, unsigned long int & tableSize);

uint64_t FNV64(string & s);

int hash64(string & key, unsigned long int & tableSize);

double productoEscalar(list<char> & review ,list<double> & listaDePesos);

int contarPalabras(string & frase);

list <string>  construirNgrama(string &, int&);

//Retorna true si la palabra es un stopWord, caso contrario retorna false
bool buscarStopWord(string &word,list<string> & listaStopWords);

//Incrementa en uno en la posicion indicada;
void incrementar(list<char> & hashTable,unsigned long int & posicion );

list<double> calcularPesos(map<string, cuerpoConHash> & diccionario, int & pasosMaximos);

map<string,cuerpoConHash> hashearNgramas(map<string,cuerpoConNgramas> & diccionario);

bool esLetra (char & c);

list <string>  crearListaDeStopWords();

map<string, cuerpoConNgramas> obtenerNgramas(map<string, cuerpoDelReview> &diccionario, int &tamanio);

map<string, cuerpoConNgramas> crearDiccionariosDeReviewsPerceptron(int &tamanio);

void crearDiccionarioInicial(map<string, cuerpoDelReview> &diccionario);

map<string, list<string> > crearDiccionariosDeReviewsDelArchAClasificar(int &tamanio);

void crearDiccionarioInicialDelArchAClasificar(map<string, string> &diccionario);

map<string, list<string> > obtenerNgramasDelArchAClasificar(map<string, string> &diccionario, int &tamanio);

map<string, list<char> > hashearNgramasDelArchAClasificar(map<string, list<string> > & diccionario);

map<string, cuerpoConHash> crearDiccionariosDeReviewsChar(int &tamanio);
