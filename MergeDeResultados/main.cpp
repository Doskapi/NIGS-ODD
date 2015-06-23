/**Inclusiones necesarias**/
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <sstream>

using namespace std;

/**Definiciones necesarias**/
#define ARCH_RESULTADOS_PERCEPTRON "archivos/resultadosPerceptron.csv"
#define ARCH_RESULTADOS_BAYES "archivos/resultadosBayes.csv"
#define ARCH_RESULTADOS_MERGE_PORCENTAJE "archivos/resultadosMergePorcentaje.csv"
#define ARCH_RESULTADOS_PERCEPTRON_ORDENADO "archivos/resultadosPerceptronOrdenado.csv"
#define ARCH_RESULTADOS_BAYES_ORDENADO "archivos/resultadosBayesOrdenado.csv"
#define ARCH_RESULTADOS_MERGE_PORCENTAJE_ORDENADO "archivos/resultadosMergePorcentajeOrdenado.csv"
#define ARCH_TEST_DATA "archivos/testData.tsv"

/**Combina dos soluciones de manera que en cada cada solucion en multiplicada por un peso.**/
void mergeDeProbabilidadesPorProrcentajeDeArchivo(const char* direccionDelArchivoDeResultadosPerceptron, const char* direccionDelArchivoDeResultadosNaiveBayes, const char* direccionDelArchivoDeResultadosMerge)
{
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

/**Dado la URL de un archivo entrada y una URL de salida se ordena de manera tal que los archivos sean ordenados
De la misma manera que testData.tsv.**/
void ordenarUnArchivo(const char* direccionDelArchivoViejo,const char* direccionDelArchivoOrdenado)
{
    ifstream archViejo;
    ifstream archTest;
    ofstream archOrdenado;
    string idViejo, clasifViejo;
    string idBay, clasifBay;
    map<string,double> mapaViejo;

    double probabilidad;

    archViejo.open(direccionDelArchivoViejo);
    if (archViejo.fail()) {
        cout << "No se ha podido abrir el archivo de clasificaciones del Perceptron!" << endl;
    }

    archOrdenado.open(direccionDelArchivoOrdenado);
    if (archOrdenado.fail()) {
        cout << "No se ha podido abrir el archivo de clasificaciones del Naive-Bayes!" << endl;
    }
    /**Borra el archivo anterior**/
    archOrdenado.clear();

    archTest.open(ARCH_TEST_DATA);
    if (archTest.fail()) {
        cout << "No se ha podido abrir el archivo de clasificaciones del TestData!" << endl;
    }

    //Extraigo el encabezado de cada archivo.
    getline(archViejo, idViejo, ',' );
    getline(archViejo, clasifViejo, '\n' );

    while (true) {

        getline(archViejo, idViejo, ',' );
        getline(archViejo, clasifViejo, '\n' );
        if (archViejo.eof()) break;
        probabilidad = (double) atof(clasifViejo.c_str());
        mapaViejo[idViejo] = probabilidad;
    }
    cout << endl;

    getline(archTest, idViejo, '\t' );
    getline(archTest, clasifViejo, '\n' );

    while(!archTest.eof()){
        getline(archTest, idViejo, '\t' );
        getline(archTest, clasifViejo, '\n');
        // escribo la primera linea del archivo
        if (idViejo != "")
        {
            archOrdenado << mapaViejo[idViejo] << endl;
        }
    }

    cout << "Archivo resultados ordenado guardado" << endl;
    cout << "en la direccion: " << direccionDelArchivoOrdenado << endl;

    archViejo.close();
    archOrdenado.close();
    archTest.close();
}

//Ordena todos los resultados para que mantengan el orden de TestData.tsv
void ordenarResultados()
{
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+            Proceso de Ordenado          +" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    cout << "Ordenando los resultados" << endl;
    cout << endl;
    ordenarUnArchivo(ARCH_RESULTADOS_PERCEPTRON,ARCH_RESULTADOS_PERCEPTRON_ORDENADO);
    ordenarUnArchivo(ARCH_RESULTADOS_BAYES,ARCH_RESULTADOS_BAYES_ORDENADO);
    ordenarUnArchivo(ARCH_RESULTADOS_MERGE_PORCENTAJE,ARCH_RESULTADOS_MERGE_PORCENTAJE_ORDENADO);
}

int main()
{
    mergeDeProbabilidadesPorProrcentajeDeArchivo(ARCH_RESULTADOS_PERCEPTRON, ARCH_RESULTADOS_BAYES , ARCH_RESULTADOS_MERGE_PORCENTAJE);
    ordenarResultados();
    return 0;
}
