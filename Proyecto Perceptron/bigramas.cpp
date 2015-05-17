//Inclusiones necesarias
#include <fstream>
#include <iostream>
#include <list>


using namespace std;

list <string>  construirBigrama(string frase){
	list <string> listaBigramas;
    string unBigrama
	int largo = frase.size();
	for (int pos=0;pos < (largo-1); pos++){
        unBigrama = frase.at(pos) + frase.at(pos + 1);
        listaBigramas.append(unBigrama);
	}
	return listaBigramas;
}

int main(int argc, char **argv) {
    string frase;
    frase = "Una frase loca cualquiera\n";
    list <string> lista = construirBigrama(frase);
    cout << lista << endl;
    return 0;
}

