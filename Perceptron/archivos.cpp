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
#include "archivos.h"


using namespace std;

list <string>  crearListaDeStopWords(){

    list <string> listaStopsWords;
    ifstream archivo;
    archivo.open("archivos/StopWords.txt");
    string palabra;
    if(archivo.fail())
    {
        cout << endl<< "Error al abrir el archivo de StopsWords" << endl << endl;
    } else {
        cout << endl << "Archivo de StopsWords abierto correctamente" << endl << endl;
        while(!archivo.eof()){
            getline(archivo, palabra,'\n' );  // Lee las palabras hasta el fin de linea
            if (palabra != "") listaStopsWords.push_back(palabra);
        }
    }
	return listaStopsWords;
}
