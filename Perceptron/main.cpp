//Inclusiones necesarias
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

list <string>  construirNgrama(string frase, int tamanioNgrama){
	list <string> listaNgramas;
    string unNgrama;
	int largo = frase.size();
	for (int pos=0;pos < (largo - tamanioNgrama); pos++){
        unNgrama = "";
        for (int i=0; (i < tamanioNgrama); i++){
            unNgrama += frase.at(pos + i);
        }
        cout << unNgrama <<endl;
        listaNgramas.push_back(unNgrama);
	}
	return listaNgramas;
}

int main()
{
    string frase = "una frase loca cualquiera en el para mostrar\n";
    list <string> listaNgramas = construirNgrama(frase, 3);
    //string ll =  (string) listaNgramas.pop_front();
    //cout << ll << endl;
    return 0;
}
