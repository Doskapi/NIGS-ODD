//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

using namespace std;

int contarPalabras(string frase){
    int cantidad = 0;
    stringstream ss(frase);
    string s;
    while (getline(ss, s, ' ')) {
        cantidad ++;
    }
    return cantidad;
}

//Dado una frase cualquiera y el tamanio del n grama devuleve
list <string>  construirNgrama(string frase, int tamanioNgrama){
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
    return 0;
}
