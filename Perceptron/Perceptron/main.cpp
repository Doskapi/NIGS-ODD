#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <time.h>
#include "funciones.h"

#define PASOS_MAXIMOS 100
#define ARCH_DE_ENTRENAMIENTO "archivos/labeledTrainData.tsv"
#define ARCH_A_CLASIFICAR "archivos/testData.tsv"
#define ARCH_RESULTADOS "archivos/resultados.csv"

using namespace std;
#include <string>
#include <vector>
#include <iterator>


void clasificar(vector<float> *listaDePesos) {
    cout << "*Procesando archivo de Reviews a clasificar... \n" << endl;
    map<string, cuerpoConLista> diccionario = crearDiccionarioDeReviews(false);
    cout << "\t-Archivo de Reviews a clasificar procesados." << endl << endl;
    //vector <float> minMax = minYMax(listaDePesos);

    map<string, float> id_y_producto;
    float prod_minimo = 0;
    float prod_maximo = 1;
    int iteracion = 0;

    for (map<string,cuerpoConLista>::iterator it = diccionario.begin(); it != diccionario.end(); ++it) {
        list < vector<int> >reviews = it->second.features;
		float producto = productoEscalar(&reviews, listaDePesos);
		id_y_producto[(string)it->first] = producto;
		if (iteracion == 0) {
            prod_minimo = producto;
            prod_maximo = producto;
		} else {
            if (producto < prod_minimo) prod_minimo = producto;
            if (producto > prod_maximo) prod_maximo = producto;
		}
		iteracion++;
    }

    ofstream archivo;
    archivo.open("archivos/Resultados.csv");
    archivo << "\"id\",\"sentiment\"\n";
    for (map<string, float>::iterator iterador = id_y_producto.begin(); iterador != id_y_producto.end(); ++iterador) {
        float probabilidad = ((iterador->second) - prod_minimo) / (prod_maximo - prod_minimo);
        archivo << (string)iterador->first << "," << probabilidad << endl;
    }
    archivo.close();
}


vector<float> entrenar() {
    int max_iteraciones = PASOS_MAXIMOS;
    cout << "*Procesando archivo de Reviews para entrenar... \n"<< endl;
    map<string, cuerpoConLista> diccionario = crearDiccionarioDeReviews(true);
    cout << "\t-Archivo de Reviews para entrenar procesados." << endl << endl;
    cout << "*Calculando pesos.. " << endl;
    vector<float> listaDePesos = calcularPesos(&diccionario, max_iteraciones);
    cout << "\t-Pesos calculados" << endl << endl;

//    cout << "*Escribiendo pesos en archivo: " << endl;
//    ofstream archListaDePesos;
//    archListaDePesos.open("archivos/listaDePesos.csv");
//    for (std::vector<float>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ) {
//        archListaDePesos << *iterador << "\n";
//    }
//    archListaDePesos.close();
//    cout << "\t-Archivo de pesos creado." << endl << endl;
    return listaDePesos;
}



int main() {
    vector<float> listaDePesos;
    char calcONoCalc;

    cout << "****************************************************\n"<< endl;
    cout << "*                    PERCEPTRON                    *\n"<< endl;
    cout << "****************************************************\n"<< endl;
    cout << "Que quiere hacer? \n" << endl;
    cout << "a) Clasificar reviews a partir de pesos pre calculados.\n" << endl;
    cout << "b) Entrenar perceptron y clasificar los reviews.\n" << endl;
    cout << "Respuesta: ";
    cin >> calcONoCalc;



    if (calcONoCalc == 'a') {
            ifstream archListaDePesos;
            vector<float>::iterator it = listaDePesos.begin();
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
            cout << "+++++++++++++++++++++++++++++++++++++++++++\n" << endl;
            cout << "+         Proceso de entrenamiento.       +\n" << endl;
            cout << "+++++++++++++++++++++++++++++++++++++++++++\n" << endl << endl;

            clock_t begin_time = clock();
            listaDePesos = entrenar();
            cout << "El entrenamiento tardo: " << float(clock() - begin_time) /  CLOCKS_PER_SEC << " segundos" << endl << endl;
    }

    cout << "+++++++++++++++++++++++++++++++++++++++++++\n" << endl;
    cout << "+          Proceso de Clasificacion       +\n" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++\n" << endl << endl;

    clock_t begin_time = clock();
    clasificar(&listaDePesos);
    cout << "La clasificacion tardo: " << float(clock() - begin_time) /  CLOCKS_PER_SEC << " segundos" << endl << endl;
    return 0;
}
