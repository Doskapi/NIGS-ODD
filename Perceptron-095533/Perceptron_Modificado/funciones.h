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
#include <vector>

using namespace std;

struct cuerpoDelReview {
    bool sentiment;
    string review;
};

struct cuerpoConNgramas {
    bool sentiment;
    list<string> listaReview;
};


struct cuerpoConLista {
    bool sentiment;
    list < std::vector<int> > features;
};


uint64_t FNV64(string & s);

int hash64(string & key, unsigned long int & tableSize);

int contarPalabras(string & frase);

list <string>  construirNgrama(string &, int&);

//Retorna true si la palabra es un stopWord, caso contrario retorna false
bool buscarStopWord(string &word,list<string> & listaStopWords);

//Incrementa en uno en la posicion indicada;
void incrementar(list<char> & hashTable,unsigned long int & posicion );

std::vector<float> calcularPesos(map<string, cuerpoConLista> & diccionario, int &pasosMaximos);

bool esLetra (char & c);

vector<string>  crearListaDeStopWords();

map<string, cuerpoConNgramas> obtenerNgramas(map<string, cuerpoDelReview> &diccionario, int &tamanio);

map<string, cuerpoConLista> crearDiccionarioDeReviews(bool entrenando);

float productoEscalar(list < std::vector<int> > *review ,std::vector<float> *listaDePesos);

vector<float> minYMax(vector<float> *vec);

