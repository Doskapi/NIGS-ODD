//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <time.h>
#include "funciones.h"

#define TAMANIO_DE_NGRAMAS 2
#define PASOS_MAXIMOS 100
#define TAMANIO_DE_LA_TABLA 10000 //33554432
#define DELTA_DE_APRENDIZAJE 0.1
    

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

string limpiarReview(string review){
    string word, reviewFiltrado = "";
    char c;
    bool esStopWord;
    list<string> listaDeStopWords = crearListaDeStopWords();

    for (unsigned int i = 0; i < review.size(); i++)
    {
        c = review[i];
        c = tolower(c);
        if(!esLetra(c))
        {
            if(word != "")
            {
                esStopWord = buscarStopWord(word, listaDeStopWords);
                if(!esStopWord)
                {
                    reviewFiltrado += word + " ";
                }
            }
            word = "";
        }
        else
        {
            word += c;
        }
    }
    return reviewFiltrado;
}

list<unsigned short int> hashDeLosNgramas(list<string> &listaNgramas) {
    unsigned long int tableSize = TAMANIO_DE_LA_TABLA;
    list<unsigned short int> hashTable(tableSize);
    unsigned long int returnValue;
    for (list<string>::iterator iterador = listaNgramas.begin(); iterador!=listaNgramas.end(); ++iterador){
        returnValue = hash32(*iterador, tableSize);
        incrementar(hashTable, returnValue);
    }
    return hashTable;
}

list<double> calcularListaDePesos(list<double> &listaDePesos, bool clasifReview, list<unsigned short int> &listaReviewHasheada, int &pasosMaximos){
    list<double>::iterator itPesos;
    list<unsigned short int>::iterator itReviews;
    int errores = 0, error, positivo;
    unsigned long int i;
    double producto;
    for(int pasos=0; pasos < pasosMaximos; pasos++){
        positivo = 0;
        producto = productoEscalar(listaReviewHasheada, listaDePesos);
        if (producto > 0.5){
            positivo = 1;
        }
        error = (int)(clasifReview) - positivo;
        if (error != 0){                //Actualizo los pesos
            errores += 1;
            itPesos = listaDePesos.begin();
            itReviews = listaReviewHasheada.begin();
            for(i= 0; i< TAMANIO_DE_LA_TABLA; i++ ){
                *itPesos += DELTA_DE_APRENDIZAJE * error * log(1.+ *itReviews);
                itPesos++;
                itReviews++;
            }
        }
        
        if (errores == 0){
            //cout << "Cantidad de errores encontrados " << errores << endl;
            return listaDePesos;
        }
        errores=0;
        pasos++;
    }
    return listaDePesos;
}

list<double> entrenar() {
    ifstream archivo;
    archivo.open("archivos/labeledTrainData.tsv");
    string id, clasificacionDelReview, review, reviewLimpio;
    int tamanioNgramas = TAMANIO_DE_NGRAMAS;
    int pasosMaximos = PASOS_MAXIMOS;
    bool clasifReview = false;
    list<string> listaReview;
    list<unsigned short int> listaReviewHasheada;
    list<double> listaDePesos(TAMANIO_DE_LA_TABLA);
    map<string, cuerpoDelReview> diccionario;



    for (list<double>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ){
        cout << *iterador;
    }
    cout << endl;   

    // lee el titulo y lo descarta
    getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
    getline(archivo, clasificacionDelReview, '\t' );
    getline(archivo, review, '\n' );
    long int j=0;
    while(!archivo.eof()){
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, clasificacionDelReview, '\t' );
        if (clasificacionDelReview == "1"){
            clasifReview = true;
        }
        getline(archivo, review, '\n' );
        reviewLimpio = limpiarReview(review);
        
        cuerpoDelReview unCuerpo;
        unCuerpo.sentiment = clasifReview;
        unCuerpo.review = reviewLimpio;
        diccionario[id] = unCuerpo;
        j++;
        cout << j << endl; 
    }
    j=0;    

    for (map<string,cuerpoDelReview>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
        clasifReview = it->second.sentiment;
        reviewLimpio = it->second.review;
        listaReview = construirNgrama(reviewLimpio,tamanioNgramas);
        //cout << "lista N gramas creada" << endl;
        listaReviewHasheada = hashDeLosNgramas(listaReview);
        //cout << "lista hasheada" << endl << endl;
        listaDePesos = calcularListaDePesos(listaDePesos,clasifReview,listaReviewHasheada,pasosMaximos);
        //cout << "Se crea la lista con los Pesos calculados" << endl << endl;
    


        j++;
        cout << j << endl;
    
//        cout << id << " " ;
//        for (list<unsigned short int>::iterator iterador = listaReviewHasheada.begin(); iterador!=listaReviewHasheada.end(); ++iterador){
//            cout << *iterador;
//        }
//        cout << endl;
    }

    ofstream archListaDePesos;
    archListaDePesos.open("archivos/listaDePesos.csv");
    for (list<double>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ){
        archListaDePesos << *iterador << "\n";
    }
    archListaDePesos.close();
    
    for (list<double>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ){
        cout << *iterador;
    }
    cout << endl;

    archivo.close();
    return listaDePesos;
}

void clasificar(list<double> &listaDePesos) {
    ifstream archivo;
    archivo.open("archivos/testData.tsv");
    string id, clasificacionDelReview, review, reviewLimpio;
    int tamanioNgramas = TAMANIO_DE_NGRAMAS;
    int pasosMaximos = PASOS_MAXIMOS;
    bool clasifReview = false;
    list<string> listaReview;
    list<unsigned short int> listaReviewHasheada;

    ofstream archivoRta;
    archivoRta.open("archivos/resultados.csv");
    archivoRta << "\"id\",\"sentiment\"\n";
    cout << "abro" << endl;
    
    getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
    getline(archivo, clasificacionDelReview, '\t' );
    getline(archivo, review, '\n' );
    long int j=0;
    while(!archivo.eof()){
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, clasificacionDelReview, '\t' );
        if (clasificacionDelReview == "1"){
            clasifReview = true;
        }
        getline(archivo, review, '\n' );
        reviewLimpio = limpiarReview(review);
        
        listaReview = construirNgrama(reviewLimpio,tamanioNgramas);
        //cout << "lista N gramas creada" << endl;
        listaReviewHasheada = hashDeLosNgramas(listaReview);
        //cout << "lista hasheada" << endl << endl;

        double rta = productoEscalar(listaReviewHasheada,listaDePesos);
        if(rta > 0.5){
            archivoRta << id << ",1\n";
        }else{
            archivoRta << id << ",0\n";
        }

        j++; cout << j << endl;

    }
    archivoRta.close();    
    archivo.close();    
}

int main() {
    //pruebas();
    list<double> listaDePesos;
    char calcONoCalc;
    cout << "Que quiere hacer?" << endl << "a. Usar vector calculado en archivos/listaDePesos.txt " << endl << "b. Calcular el vector otra vez " << endl << "Rta: ";
    cin >> calcONoCalc;
    cout << endl;

    if (calcONoCalc == 'a'){

            ifstream archListaDePesos;
            archListaDePesos.open("archivos/listaDePesos.csv");
            list<double>::iterator it = listaDePesos.begin();
            double dato;
            string datoString;
            while(!archListaDePesos.eof()){
                //getline(archListaDePesos, datoString,'\n');
                archListaDePesos >> dato;
                it = listaDePesos.end();
                listaDePesos.insert(it,dato);
            }
            int i = 0;
            for (list<double>::iterator iterador = listaDePesos.begin(); iterador != listaDePesos.end(); iterador++ ){
                cout << i << ") " ;
                cout << *iterador << endl;
                i++;
            }
            archListaDePesos.close();

    }else{
            clock_t begin_time = clock();
            cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
            cout << "Proceso de entrenamiento..." << endl;
            cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
            listaDePesos = entrenar();
            cout << "El entrenamiento tardo: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl << endl;
    }


    clock_t begin_time = clock();
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Clasificacion de reviews..." << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    clasificar(listaDePesos);
    cout << "La clasificacion tardo: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl << endl;

    return 0;
}
