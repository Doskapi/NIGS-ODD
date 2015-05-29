//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <time.h>
#include "funciones.h"

#define TAMANIO_DE_NGRAMAS 2
#define PASOS_MAXIMOS 40
#define ARCH_DE_ENTRENAMIENTO "archivos/labeledTrainData.tsv"
#define ARCH_A_CLASIFICAR "archivos/unlabeledTrainData.tsv"
#define ARCH_RESULTADOS "archivos/resultados.csv"

using namespace std;

/* void pruebas(){ // aca se hace las pruebas
    string frase = "una frase loca para mostrar que este n grama si funciona correctamente saludos";
    list<string> listaNgramas = construirNgrama(frase, 3);
    list<string>::iterator iterador; //Esta es la forma de recorrer esta lista
    int i = 0;
    for (iterador = listaNgramas.begin(); iterador != listaNgramas.end(); iterador++ ){
        cout << i << ") " ;
        cout << *iterador << endl;
        i++;
    }
    list<string> listaStopWords = crearListaDeStopWords();
    i = 0;
    for (iterador = listaStopWords.begin(); iterador != listaStopWords.end(); iterador++ ){
        cout << i << ") " ;
        cout << *iterador << endl;
        i++;
    }

    cout << buscarStopWord("to",listaStopWords) << endl;

    string stringToHash;
    uint32_t returnValue32;
    unsigned long int tableSize = 20;//33554432; //deberia ser 2^25 = 33554432
    //unsigned long int hashTable[tableSize];
    list<unsigned short int> hashTable (tableSize);
    list<unsigned short int>::iterator iter; //Esta es la forma de recorrer esta lista
    //for (iter = hashTable.begin(); iter != hashTable.end(); iter++ ){
    //    *iter = 0;
    //}
    for (int i = 0; i < 3; ++i){
        cout << endl;
        cout << "Prueba de Hash" << endl;
        cout << "=============================================" << endl;
        cout << "Inserte string a hashear: ";
        cin >> stringToHash;
        cout << endl;

        returnValue32 = hash32(stringToHash, tableSize);
        cout << "Hash32: " << returnValue32 << endl;
        incrementar(hashTable, returnValue32);
    }
    cout << "[pos]:hashvalue" << endl;
    int j = 0;
    for (iter = hashTable.begin(); iter != hashTable.end(); iter++ ){
        cout << "["<< j << "]:" << *iter << "  " << endl;
        if ((j%9 == 0) && (j>1)){
            cout << endl;
        }
    }
    cout << endl;
}
*/


list<double> entrenar() {
    int tamanioNgramas = TAMANIO_DE_NGRAMAS;
    map<string, cuerpoConHash> diccionario = crearDiccionariosDeReviewsChar(tamanioNgramas);
    cout << "Diccionario creado con N gramas" << endl << endl;
    //map<string, cuerpoConHash> diccionarioHasheado = hashearNgramas(diccionario);
    //diccionario.clear();
    cout << "Diccionario hasheado" << endl << endl;
    int pasosMaximos = PASOS_MAXIMOS;
    list<double> listaDePesos = calcularPesos(diccionario,pasosMaximos);
    cout << "Se crea la lista con los Pesos calculados" << endl << endl;

//    int i = 0;
//    for (list<double>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ){
//        cout << i << ") " ;
//        cout << *iterador << endl;
//        i++;
//    }

    return listaDePesos;
}

void clasificar(list<double> listaDePesos) {

    //primero tengo q limpiar el review --> listo
    int tamanioNgramas = TAMANIO_DE_NGRAMAS;
    map<string, list<string> > diccionario = crearDiccionariosDeReviewsDelArchAClasificar(tamanioNgramas);
    cout << "Diccionario creado con N gramas del archivo de reviews a clasificar" << endl << endl;

    //segundo generar los ngramas --> listo
    map<string, list<char> > diccionarioHasheado = hashearNgramasDelArchAClasificar(diccionario);
    cout << "Diccionario hasheado del archivo de reviews a clasificar" << endl << endl;

    //abro el archivo de resultados
    fstream archivo;
    archivo.open("archivos/resultados.csv");
    archivo << "\"id\",\"sentiment\"\n";

    // por cada review
    for (map<string, list<char> >::iterator it=diccionarioHasheado.begin(); it!=diccionarioHasheado.end(); ++it){
        double rta = productoEscalar((it->second),listaDePesos);
        if(rta > 0.5){
            archivo << (string)it->first << ",1\n";
        }else{
            archivo << (string)it->first << ",0\n";
        }
    }
    archivo.close();
}

int main() {
    //pruebas();
    clock_t begin_time = clock();
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Proceso de entrenamiento..." << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    list<double> listaDePesos = entrenar();
    cout << "El entrenamiento tardo: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl << endl;

    begin_time = clock();
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Clasificacion de reviews..." << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    clasificar(listaDePesos);
    cout << "La clasificacion tardo: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl << endl;

    return 0;
}
