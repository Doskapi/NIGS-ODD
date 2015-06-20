#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <sstream>

using namespace std;

#define ARCH_RESULTADOS_PERCEPTRON "archivos/resultadosPerceptron.csv"
#define ARCH_RESULTADOS_BAYES "archivos/resultadosBayes.csv"
#define ARCH_RESULTADOS_MERGE_PORCENTAJE "archivos/resultadosMergePorcentaje.csv"
#define ARCH_RESULTADOS_MERGE_DISTANCIA "archivos/resultadosMergeDistancia.csv"


void mergeDeProbabilidadesPorProrcentajeDeArchivo(const char* direccionDelArchivoDeResultadosPerceptron, const char* direccionDelArchivoDeResultadosNaiveBayes, const char* direccionDelArchivoDeResultadosMerge) {
    ifstream archPerceptron;
    ifstream archNaiveBayes;
    ofstream archResultados;
    string idPer, clasifPer;
    string idBay, clasifBay;
    map<string,double> mapaPer, mapaBay;

    double probabilidadPer, probabilidadBay, resultado;

    double porcentajeEnteroPer, porcentajeEnteroBay;
    double porcentajePer, porcentajeBay;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+            Proceso de Merge             +" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    cout << "Que porcentaje quiere darle a Perceptron? (entre  y 100, numero entero)" << endl;
    cout << "Porcentaje(%): ";
    cin >> porcentajeEnteroPer;
    cout << endl;
    cout << "El porcentaje(%) del Naive-Bayes sera: " << 100.0 - porcentajeEnteroPer << endl;
    porcentajeEnteroBay = 100.0 - porcentajeEnteroPer;
    porcentajePer = (double)porcentajeEnteroPer/100.0;
    porcentajeBay = (double)porcentajeEnteroBay/100.0;


    archPerceptron.open(direccionDelArchivoDeResultadosPerceptron);
    if (archPerceptron.fail()) {
        cout << "No se ha podido abrir el archivo de clasificaciones del Perceptron!" << endl;
    }

    archNaiveBayes.open(direccionDelArchivoDeResultadosNaiveBayes);
    if (archNaiveBayes.fail()) {
        cout << "No se ha podido abrir el archivo de clasificaciones del Naive-Bayes!" << endl;
    }

    archResultados.open(direccionDelArchivoDeResultadosMerge);
    archResultados.clear();

    //Extraig   o el encabezado de cada archivo.
    getline(archPerceptron, idPer, ',' );
    getline(archNaiveBayes, idBay, ',' );
    getline(archPerceptron, clasifPer, '\n' );
    getline(archNaiveBayes, clasifBay, '\n' );

    // escribo la primera linea del archivo
    archResultados << idPer << "," << clasifPer << endl;

    while (true) {

        getline(archPerceptron, idPer, ',' );
        getline(archPerceptron, clasifPer, '\n' );
        if (archPerceptron.eof()) break;
        probabilidadPer = (double) atof(clasifPer.c_str());
        mapaPer[idPer] = probabilidadPer;
    }

    while (true) {

        getline(archNaiveBayes, idBay, ',' );
        getline(archNaiveBayes, clasifBay, '\n' );
        if (archNaiveBayes.eof()) break;
        probabilidadBay = (double) atof(clasifBay.c_str());
        mapaBay[idBay] = probabilidadBay;
    }

    map<string,double>::iterator itPer = mapaPer.begin();
    map<string,double>::iterator itBay = mapaBay.begin();

    cout << endl;

    int cantPos = 0 , cantNeg = 0 , cantTotal;

    while( itPer != mapaPer.end() ){
        cantTotal ++;

        resultado = porcentajePer * (itPer->second) + porcentajeBay * (itBay->second);

        if (resultado >= 0.5){
            cantPos++;
        }   else {
            cantNeg++;
        }

        if ((resultado > 1)||(resultado < 0)){
            cout << "alerta" << endl;
        }

        if (itPer->first != itBay->first){
            cout << itPer->first << itPer->second << "  " << itBay->first << itBay->second << "  " << resultado << endl;
        }


        archResultados << itPer->first << "," << resultado << endl;

        itPer++;
        itBay++;
    }

    cout << "Archivo merge generado.\n" << endl;

    cout << "Estadistica: " << cantPos << " " << cantNeg << " " << cantTotal << endl;

    archPerceptron.close();
    archNaiveBayes.close();
    archResultados.close();

}


void mergeDeProbabilidadesSegunDistancia(const char* direccionDelArchivoDeResultadosPerceptron, const char* direccionDelArchivoDeResultadosNaiveBayes, const char* direccionDelArchivoDeResultadosMerge) {
    ifstream archPerceptron;
    ifstream archNaiveBayes;
    ofstream archResultados;
    string idPer, clasifPer;
    string idBay, clasifBay;
    map<string,float> mapaPer, mapaBay;
    float probabilidadPer ,probabilidadBay;

    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+     Proceso de Merge por Distancias     +" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;

    archPerceptron.open(direccionDelArchivoDeResultadosPerceptron);
    if (archPerceptron.fail()) {
        cout << "No se ha podido abrir el archivo de clasificaciones del Perceptron!" << endl;
    }

    archNaiveBayes.open(direccionDelArchivoDeResultadosNaiveBayes);
    if (archNaiveBayes.fail()) {
        cout << "No se ha podido abrir el archivo de clasificaciones del Naive-Bayes!" << endl;
    }

    archResultados.open(direccionDelArchivoDeResultadosMerge);
    archResultados.clear();

    //Extraigo el encabezado de cada archivo.
    getline(archPerceptron, idPer, ',' );
    getline(archNaiveBayes, idBay, ',' );
    getline(archPerceptron, clasifPer, '\n' );
    getline(archNaiveBayes, clasifBay, '\n' );

    // escribo la primera linea del archivo
    archResultados << idPer << "," << clasifPer << endl;

    while (true) {
        if (archPerceptron.eof()) break;
        getline(archPerceptron, idPer, ',' );
        getline(archPerceptron, clasifPer, '\n' );
        probabilidadPer = (float) atof(clasifPer.c_str());
        mapaPer[idPer] = probabilidadPer;
    }

    while (true) {
        if (archNaiveBayes.eof()) break;
        getline(archNaiveBayes, idBay, ',' );
        getline(archNaiveBayes, clasifBay, '\n' );
        probabilidadBay = (float) atof(clasifBay.c_str());
        mapaBay[idBay] = probabilidadBay;
    }

    map<string,float>::iterator itPer = mapaPer.begin();
    map<string,float>::iterator itBay = mapaBay.begin();
    float difPer, difBay;
    cout << endl;

    while( itPer != mapaPer.end() ){
        if ( ((itPer->second >= 0.5) && (itBay->second < 0.5)) || ((itBay->second >= 0.5) && (itPer->second < 0.5)) ){
            if ((itPer->second >= 0.5) && (itBay->second < 0.5)) {
                difPer = itPer->second - 0.5; //pos
                difBay = 0.5 - itBay->second;  //neg
                if(difPer > difBay){
                    //difPer es la mas lejos
                    archResultados << itPer->first << "," << itPer->second << endl;
                }else{
                    // difBay es la mas lejos
                    archResultados << itBay->first << "," << itBay->second << endl;
                }
            }else{
                difBay = itBay->second - 0.5; //pos
                difPer = 0.5 - itPer->second;  //neg
                if(difPer > difBay){
                    //difPer es la mas lejos
                    archResultados << itPer->first << "," << itPer->second << endl;
                }else{
                    // difBay es la mas lejos
                    archResultados << itBay->first << "," << itBay->second << endl;
                }
            }
        }else{
            // escribo uno de los dos total son iguales
            archResultados << itPer->first << "," << itPer->second << endl;
        }
        itPer++;
        itBay++;
    }

    cout << "Archivo merge generado.\n" << endl;

    archPerceptron.close();
    archNaiveBayes.close();
    archResultados.close();

}


int main()
{
    mergeDeProbabilidadesPorProrcentajeDeArchivo(ARCH_RESULTADOS_PERCEPTRON, ARCH_RESULTADOS_BAYES , ARCH_RESULTADOS_MERGE_PORCENTAJE);
    mergeDeProbabilidadesSegunDistancia(ARCH_RESULTADOS_PERCEPTRON, ARCH_RESULTADOS_BAYES , ARCH_RESULTADOS_MERGE_DISTANCIA);

    return 0;
}
