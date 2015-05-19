//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include "funciones.h"
#include "archivos.h"

using namespace std;


int main() {
    string frase = "una frase loca para mostrar que este n grama si funciona correctamente saludos";
    list<string> listaNgramas = construirNgrama(frase, 3);
    list<string>::iterator iterador; //Esta es la forma de recorrer esta lista
    int i = 0;
    for( iterador = listaNgramas.begin(); iterador != listaNgramas.end(); iterador++ ){
        cout << i << ") " ;
        cout << *iterador << endl;
        i++;
    }
    list<string> listaStopWords = crearListaDeStopWords();
    i = 0;
    for( iterador = listaStopWords.begin(); iterador != listaStopWords.end(); iterador++ ){
        cout << i << ") " ;
        cout << *iterador << endl;
        i++;
    }
    return 0;
}
