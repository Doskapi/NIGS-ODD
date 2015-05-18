//Inclusiones necesarias
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

//Dado una frase cualquiera y el tamanio del n grama devuleve
list <string>  construirNgrama(string frase, int tamanioNgrama){
	list <string> listaNgramas;
	string unNgrama;
	int largo = frase.size();
	for (int pos=0;pos < (largo - tamanioNgrama); pos++){
        unNgrama = "";
        for (int i=0; (i < tamanioNgrama); i++){
            unNgrama += frase.at(pos + i);
        }
        listaNgramas.push_back(unNgrama);
	}
	return listaNgramas;
}

int main()
{
    string frase = "una frase loca cualquiera en el para mostrar\n";
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
