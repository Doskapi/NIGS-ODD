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
#define ARCH_RESULTADOS_MERGE_PORCENTAJE "archivos/resultadosMerge.csv"


void mergeDeProbabilidadesPorProrcentajeDeArchivo(const char* direccionDelArchivoDeResultadosPerceptron, const char* direccionDelArchivoDeResultadosNaiveBayes, const char* direccionDelArchivoDeResultadosMerge) {
    ifstream archPerceptron;
    ifstream archNaiveBayes;
    ofstream archResultados;
    string idPer, clasifPer;
    string idBay, clasifBay;
    map<string,float> mapaPer, mapaBay;

    float probabilidadPer, probabilidadBay, resultado;

    int porcentajeEnteroPer, porcentajeEnteroBay;
    float porcentajePer, porcentajeBay;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+            Proceso de Merge             +" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    cout << "Que porcentaje quiere darle a Perceptron? (entre  y 100, numero entero)" << endl;
    cout << "Porcentaje(%): ";
    cin >> porcentajeEnteroPer;
    cout << endl;
    cout << "El porcentaje(%) del Naive-Bayes sera: " << 100-porcentajeEnteroPer << endl;
    porcentajeEnteroBay = 100-porcentajeEnteroPer;
    porcentajePer = (float)porcentajeEnteroPer/100;
    porcentajeBay = (float)porcentajeEnteroBay/100;


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

    cout << endl;

    while( itPer != mapaPer.end() ){

        resultado = porcentajePer * (itPer->second) + porcentajeBay * (itBay->second);
        if (itPer->first != itBay->first){
            cout << itPer->first << itPer->second << "  " << itBay->first << itBay->second << "  " << resultado << endl;
        }

        archResultados << itPer->first << "," << resultado << endl;

        itPer++;
        itBay++;
    }

    cout << "Archivo merge generado.\n" << endl;

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

    float probabilidadPer, probabilidadBay, resultado;

    int porcentajeEnteroPer, porcentajeEnteroBay;
    float porcentajePer, porcentajeBay;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+            Proceso de Merge             +" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    cout << "Que porcentaje quiere darle a Perceptron? (entre  y 100, numero entero)" << endl;
    cout << "Porcentaje(%): ";
    cin >> porcentajeEnteroPer;
    cout << endl;
    cout << "El porcentaje(%) del Naive-Bayes sera: " << 100-porcentajeEnteroPer << endl;
    porcentajeEnteroBay = 100-porcentajeEnteroPer;
    porcentajePer = (float)porcentajeEnteroPer/100;
    porcentajeBay = (float)porcentajeEnteroBay/100;


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

    cout << endl;

    while( itPer != mapaPer.end() ){

        resultado = porcentajePer * (itPer->second) + porcentajeBay * (itBay->second);
        if (itPer->first != itBay->first){
            cout << itPer->first << itPer->second << "  " << itBay->first << itBay->second << "  " << resultado << endl;
        }

        archResultados << itPer->first << "," << resultado << endl;

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
    mergeDeProbabilidadesSegunDistancia(ARCH_RESULTADOS_PERCEPTRON, ARCH_RESULTADOS_PERCEPTRON, ARCH_RESULTADOS_MERGE_PORCENTAJE);
    return 0;
}
