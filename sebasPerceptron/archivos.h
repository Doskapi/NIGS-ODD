
//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
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
    list<unsigned short int> hashTable;
};

bool esLetra (char c);

list <string>  crearListaDeStopWords();

void crearDiccionarioInicial(map<string, cuerpoDelReview> &diccionario);

map<string, cuerpoConNgramas> obtenerNgramas(map<string, cuerpoDelReview> &diccionario, int tamanio);

map<string,cuerpoConNgramas> crearDiccionariosDeReviewsPerceptron(int tamanio);

