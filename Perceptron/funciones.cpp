
//Inclusiones necesarias
#include "funciones.h"

//Constantes para las funciones hash
#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U
#define FNV_PRIME_64 1099511628211
#define FNV_OFFSET_64 14695981039346656037U

#define DELTA_DE_APRENDIZAJE 0.1

using namespace std;


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

double productoEscalar(list<unsigned short int> review ,list<double> listaDePesos) {
    double prod = 0;
    list<unsigned short int>::iterator itReview = review.begin();
    list<double>::iterator itPesos = listaDePesos.begin();
    for(unsigned long int i = 0; i < TAMANIO_DE_LA_TABLA; i++){
        prod += (*itPesos) * ((double) *itReview) ;
        itPesos++;
        itReview++;
    }
    return prod;
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

list<double> calcularPesos(map<string, cuerpoConHash> & diccionario, unsigned long int tableSize, int pasosMaximos){
    list<double> listaDePesos (tableSize);
    list<double>::iterator itPesos;
    list<unsigned short int>::iterator itReviews;
    int errores = 0, error, positivo;
    unsigned long int i;
    double producto;
    for(int pasos=0; pasos < pasosMaximos; pasos ++){
        for (map<string,cuerpoConHash>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
            list<unsigned short int> reviews = it->second.hashTable;
            positivo = 0;
			producto = productoEscalar(reviews, listaDePesos);
            if (producto > 0.5) positivo = 1;
			error = (int)(it->second.sentiment) - positivo;
			if (error != 0){                //Actualizo los pesos
				errores += 1;
				itPesos = listaDePesos.begin();
				itReviews = reviews.begin();
				for(i= 0; i< tableSize; i++ ){
					*itPesos += DELTA_DE_APRENDIZAJE * error * log(1.+ *itReviews);
					itPesos++;
					itReviews++;
				}
			}
        }
        if (errores == 0){
			cout << "Cantidad de errores encontrados " << errores << endl;
            return listaDePesos;
        }
        errores=0;
        pasos++;
    }
    return listaDePesos;
}

