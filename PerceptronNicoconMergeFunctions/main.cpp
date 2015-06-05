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
            } else if ((itPer->second <= 0.5)&&(!itBay->second.sentiment)) {
                archivo << (string)itPer->first << ",0\n";
                coinciden++;
            } else {
                if( itPer->second >  0.5){
                    archivo << (string)itPer->first << ",0\n";
                } else {
                    archivo << (string)itPer->first << ",1\n";
                }
                difieren++;
            }
            itPer++;
            itBay++;
		} else {
            itPer++;
		}
    }
    cout << "Coinciden en: " << coinciden << endl;
    cout << "Difieren en: " << difieren << endl;
}


void mergearResultados2(map<string,double> &mapaPercepton){
    map<string,cuerpoConProb> mapaBayes = cargarResultadosBayes();
    transformarProbabilidadesPerceptron(mapaPercepton);

    map<string,double>::iterator itPer = mapaPercepton.begin();
    map<string,cuerpoConProb>::iterator itBay = mapaBayes.begin();

    double resultado;
    ofstream archivo;
    archivo.open("archivos/ResultadosConBayes.csv");
    archivo.clear();
    archivo << "\"id\",\"sentiment\"\n";

    while( itPer != mapaPercepton.end() ){
		if (itPer->first != ""){
            resultado = 0.55 * (itPer->second) + 0.45 * (itBay->second.probabilidad);

                if(resultado >  0.5){
                    archivo << (string)itPer->first << ",1\n";
                }else{
                    archivo << (string)itPer->first << ",0\n";
                }
            itPer++;
            itBay++;
		} else {
            itPer++;
		}
    }
}

void clasificar(std::vector<float> listaDePesos) {

    //primero tengo q limpiar el review --> listo
    int tamanioNgramas = TAMANIO_DE_NGRAMAS;
    map<string, cuerpoConLista> diccionario = crearDiccionarioDeReviews(tamanioNgramas, false);
    cout << "Diccionario creado con N gramas del archivo de reviews a clasificar" << endl << endl;

    //segundo generar los ngramas --> listo
    //map<string, list<char> > diccionarioHasheado = hashearNgramasDelArchAClasificar(diccionario);
    //cout << "Diccionario hasheado del archivo de reviews a clasificar" << endl << endl;

    //abro el archivo de resultados
    ofstream archivo;
    archivo.open("archivos/Resultados.csv");
    archivo << "\"id\",\"sentiment\"\n";

    /************************/
    map<string,double> mapaPercepton;

    /************************/

    for (map<string,cuerpoConLista>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
        list < std::vector<int> >reviews = it->second.features;
		float producto = productoEscalar(&reviews, &listaDePesos);
		if (it->first != ""){
            if(producto >  0.5){
                archivo << (string)it->first << ",1\n";
            }else{
                archivo << (string)it->first << ",0\n";
            }
            mapaPercepton[it->first] = producto;
		}
    }
    mergearResultados2(mapaPercepton);
    archivo.close();
}



std::vector<float> entrenar() {
    int tamanio_ngrama = TAMANIO_DE_NGRAMAS;
    int max_iteraciones = PASOS_MAXIMOS;

    map<string, cuerpoConLista> diccionario = crearDiccionarioDeReviews(tamanio_ngrama, true);
    cout << "Diccionario de N gramas creado.\n" << endl << endl;

    std::vector<float> listaDePesos = calcularPesos(diccionario, max_iteraciones);
    cout << "Lista de Pesos calculada. \n" << endl << endl;

    //Clasifico directamente aca porque tengo un problema de Segmentation Fault en Clasificar()
    clasificar(listaDePesos);

    ofstream archListaDePesos;
    archListaDePesos.open("archivos/listaDePesos.csv");
    for (std::vector<float>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ) {
        archListaDePesos << *iterador << "\n";
    }
    archListaDePesos.close();

    return listaDePesos;
}



int main() {
    std::vector<float> listaDePesos;
    char calcONoCalc;
    cout << "Que quiere hacer?" << endl << "a. Usar vector calculado en archivos/listaDePesos.txt "
        << endl << "b. Calcular el vector otra vez " << endl << "Rta: ";
    cin >> calcONoCalc;
    cout << endl;

    if (calcONoCalc == 'a') {
            ifstream archListaDePesos;
            std::vector<float>::iterator it = listaDePesos.begin();
            double dato;
            string datoString;
            while(!archListaDePesos.eof()){
                //getline(archListaDePesos, datoString,'\n');
                archListaDePesos >> dato;
                it = listaDePesos.end();
                listaDePesos.insert(it,dato);
            }
            archListaDePesos.open("archivos/listaDePesos.csv");
            int i = 0;
            for (std::vector<float>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ){
                cout << i << ") " ;
                cout << *iterador << endl;
                i++;
            }
            archListaDePesos.close();

    } else {
            clock_t begin_time = clock();
            cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "Proceso de entrenamiento..." << endl;
            cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
            std::vector<float> listaDePesos = entrenar();
            cout << "El entrenamiento tardo: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl << endl;
    }


    clock_t begin_time = clock();
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Clasificacion de reviews..." << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    //clasificar(listaDePesos);
    cout << "La clasificacion tardo: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl << endl;
    return 0;
}
