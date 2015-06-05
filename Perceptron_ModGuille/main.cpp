#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <time.h>
#include "funciones.h"
#include <stdlib.h>

#define TAMANIO_DE_NGRAMAS 1
#define PASOS_MAXIMOS 100
#define ARCH_DE_ENTRENAMIENTO "archivos/labeledTrainData.tsv"
#define ARCH_A_CLASIFICAR "archivos/testData.tsv"
#define ARCH_RESULTADOS "archivos/resultados.csv"

using namespace std;



//#include <iostream>
#include <string>
//#include <sstream>
#include <vector>
#include <iterator>

double maximoDelMapaBayes(map<string,cuerpoConProb> &lista){
    map<string,cuerpoConProb>::iterator it;
    double maximo = 0;
    for (it = lista.begin(); it != lista.end(); ++it){
            if(it->second.probabilidad > maximo){
                maximo = it->second.probabilidad;
            }
    }
    return maximo;
}

double maximoDelMapaPerceptron(map<string,double> &lista){
    map<string,double>::iterator it;
    double maximo = 0;
    for (it = lista.begin(); it != lista.end(); ++it){
            if(it->second > maximo){
                maximo = it->second;
            }
    }
    return maximo;
}

double minimoDelMapaBayes(map<string,cuerpoConProb> &lista){
    map<string,cuerpoConProb>::iterator it;
    double minimo = 0;
    for (it = lista.begin(); it != lista.end(); ++it){
            if(it->second.probabilidad < minimo){
                minimo = it->second.probabilidad;
            }
    }
    return minimo;
}

double minimoDelMapaPerceptron(map<string,double> &lista){
    map<string,double>::iterator it;
    double minimo = 0;
    for (it = lista.begin(); it != lista.end(); ++it){
            if(it->second < minimo){
                minimo = it->second;
            }
    }
    return minimo;
}

void transformarProbabilidadesBayes(map<string,cuerpoConProb> & mapa){
    double maximo , minimo;
    maximo = maximoDelMapaBayes(mapa);
    minimo = minimoDelMapaBayes(mapa);
    map<string,cuerpoConProb>::iterator it;
    for (it = mapa.begin(); it != mapa.end(); ++it){
            it->second.probabilidad = (it->second.probabilidad - minimo)/(maximo - minimo);
    }
}

void transformarProbabilidadesPerceptron(map<string,double> & mapa){
    double maximo = maximoDelMapaPerceptron(mapa);
    double minimo = minimoDelMapaPerceptron(mapa);
    cout << maximo << " " << minimo << endl;
    map<string,double>::iterator it;
    maximo = 1;
    minimo = 0;
    for (it = mapa.begin(); it != mapa.end(); ++it){
            it->second = (it->second - minimo)/(maximo - minimo);
    }
}

map<string,cuerpoConProb> cargarResultadosBayes(){
    ifstream archivo;
    cuerpoConProb unCuerpo;
    map<string,cuerpoConProb> mapa;
    bool esPositivo;
    string id, p ,sentiment;

    archivo.open("archivos/probabilidadesBayes.csv");
    if (archivo.fail()) {
        cout << "ERROR! No se pudo encontrar el archivo." << endl;
    }
    while (!archivo.eof()) {
        esPositivo = false;
        getline(archivo, id, ',' );
        getline(archivo, p, ',' );
        getline(archivo, sentiment, '\n' );
        if (sentiment == "1") esPositivo = true;
        std::string::size_type sz;     // alias of size_t
        unCuerpo.probabilidad = atof( p.c_str() );//strtod(p,sz);
        unCuerpo.sentiment = esPositivo;
        mapa[id] = unCuerpo;
    }
    transformarProbabilidadesBayes(mapa);
    archivo.close();
    return mapa;
}

void mergearResultados(map<string,double> &mapaPercepton){
    map<string,cuerpoConProb> mapaBayes = cargarResultadosBayes();
    transformarProbabilidadesPerceptron(mapaPercepton);

    map<string,double>::iterator itPer = mapaPercepton.begin();
    map<string,cuerpoConProb>::iterator itBay = mapaBayes.begin();

    int coinciden = 0, difieren = 0;

    double resultado;
    ofstream archivo;
    archivo.open("archivos/ResultadosFinal.csv");
    archivo.clear();
    archivo << "\"id\",\"sentiment\"\n";

    while( itPer != mapaPercepton.end() ){
		if (itPer->first != ""){
            if((itPer->second > 0.5)&&(itBay->second.sentiment)){
                archivo << (string)itPer->first << ",1\n";
                coinciden++;
            } else if