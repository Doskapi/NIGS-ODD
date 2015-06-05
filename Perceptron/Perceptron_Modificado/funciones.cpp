#include "funciones.h"
#include <vector>
#include <ctype.h>

#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U
#define FNV_PRIME_64 1099511628211
#define FNV_OFFSET_64 14695981039346656037U
#define DELTA_DE_APRENDIZAJE 0.1
#define TAMANIO_DE_LA_TABLA 33554432
#define FILTRO 50000 // para ver menos cosas

using namespace std;


uint64_t FNV64(string &s) {
    // Hash FNV 64
    int a = s.size();
    uint64_t hash = FNV_OFFSET_64;
    for(int i = 0; i < a; i++) {
        hash = hash ^ s[i];
        hash = hash * FNV_PRIME_64;
    }
    return hash;
}


int hash64(string &key, unsigned long int &tableSize) {
    int index;
    index = FNV64(key);
    index = index%tableSize;
    return index;
}



void unir_listas(vector<string> *lista_1, vector<string> *lista_2) {
    int tamanio_2 = (*lista_2).size();
    for (int indice = 0; indice < tamanio_2; indice++) {
        (*lista_1).push_back((*lista_2)[indice]);
    }
}


vector<float> minYMax(vector<float> *vec){
    vector<float> minMax(2);
    for (int i =0 ; i < (*vec).size() ; i++){
        if (i==0){
            minMax[0] = (*vec)[i];
            minMax[1] = (*vec)[i];
        }else{
            if(minMax[0] < (*vec)[i]) minMax[0] = (*vec)[i];
            if(minMax[1] > (*vec)[i]) minMax[1] = (*vec)[i];
        }
    }
    return minMax;
}


float productoEscalar(list < std::vector<int> > *review ,std::vector<float> *listaDePesos) {
    float prod = 0;
    list < std::vector<int> >::iterator itReview;
    for(itReview = (*review).begin();itReview!=(*review).end(); ++itReview){
        prod += (float)((*itReview)[1]) * (*listaDePesos)[((*itReview)[0])] ;
    }
    return prod;
}


vector<float> calcularPesos(map<string, cuerpoConLista> & diccionario, int &max_iteraciones){

    list <vector<int> > reviews;
    vector<float> listaDePesos (TAMANIO_DE_LA_TABLA, 0);
    list <vector<int> >::iterator itReviews;
    int positivo;
    float producto;

    for(int iteracion = 0; iteracion < max_iteraciones; iteracion++) {
        int errores = 0;
        for (map<string,cuerpoConLista>::iterator it = diccionario.begin(); it != diccionario.end(); ++it) {
            reviews = it->second.features;
			producto = productoEscalar(&reviews, &listaDePesos);
            if (producto > 0.5) {
                positivo = 1;
            } else {
                positivo = 0;
            }
			int error = (int)(it->second.sentiment) - positivo;
			if (error != 0) {
				errores += 1;
				for (itReviews = reviews.begin(); itReviews != reviews.end() ; ++itReviews) {
                    listaDePesos[(*itReviews)[0]] += DELTA_DE_APRENDIZAJE * error * abs(log(1. + (*itReviews)[1]));
				}
			}
        }
        cout << "\t-Iteracion: "<< iteracion << " Errores encontrados: " << errores <<"\n" << endl;
        if (errores == 0) {
            return listaDePesos;
        }
    }
    return listaDePesos;
}


int contarPalabras(string &frase) {
    int cantidad = 0;
    stringstream ss(frase);
    string s;
    while (getline(ss, s, ' ')) {
        cantidad ++;
    }
    return cantidad;
}



vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


vector<string> construirNGrama(vector<string> *lista_palabras, int tamanio_ngrama) {
    int cant_palabras = (*lista_palabras).size();
    vector<string> lista_ngramas;

    for (int indice = 0; indice <= (cant_palabras - tamanio_ngrama); indice++) {
            string n_grama = "";
            for (int sub_indice = 0; sub_indice < tamanio_ngrama; sub_indice++) {
                n_grama += " " + ((*lista_palabras)[(indice + sub_indice)]);
            }
            lista_ngramas.push_back(n_grama);
    }
    return lista_ngramas;
}



vector<string>  crearListaDeStopWords() {
    vector<string> listaStopsWords;
    ifstream archivo;
    archivo.open("archivos/StopWords.txt");
    string palabra;
    if (archivo.fail()) {
        cout << endl<< "Error al abrir el archivo de StopsWords" << endl << endl;
    } else {
        while(!archivo.eof()){
            getline(archivo, palabra,'\n' );  // Lee las palabras hasta el fin de linea
            if (palabra != "") listaStopsWords.push_back(palabra);
        }
    }
	return listaStopsWords;
}


bool es_StopWord(string palabra, vector<string> lista_de_StopWords) {
    int izquierda = 0;
    int derecha = lista_de_StopWords.size() - 1;
    int punto_medio = (izquierda + derecha) / 2;

    while (izquierda <= derecha) {
        if (palabra == lista_de_StopWords[punto_medio]) {
            return true;
        } else {
            if (palabra > lista_de_StopWords[punto_medio]) {
                izquierda = punto_medio;
            } else {
                derecha = punto_medio;
            }
        }
        punto_medio = (izquierda + derecha) / 2;
    }
    return false;
}


string filtrar_review(string *review, bool eliminar_StopWords) {
    string cadena = "";
    string review_filtrado = "";
    vector<string> listaDeStopWords;

    if (eliminar_StopWords) {
        vector<string> listaDeStopWords = crearListaDeStopWords();
    }

    for (unsigned int indice = 0; indice < (*review).size(); indice++) {
        char caracter = tolower((*review)[indice]);
        if (!isalpha(caracter) && !isdigit(caracter)) {
            if (cadena != "") {
                if (eliminar_StopWords) {
                    if (!es_StopWord(cadena, listaDeStopWords)) {
                        review_filtrado += cadena + " ";
                    }
                } else {
                    review_filtrado += cadena + " ";
                }
                cadena = "";
            }
        } else {
            cadena += caracter;
        }
    }
    return review_filtrado;
}


map<string, cuerpoConLista> crearDiccionarioDeReviews(bool entrenando) {
    unsigned long int dimensiones = TAMANIO_DE_LA_TABLA;
    unsigned long int returnValue;
    map<string, cuerpoConLista> diccionario;
    string id, clasificacionDelReview, review;

    const char* ruta_archivo;
    if (entrenando) {
        ruta_archivo = "archivos/labeledTrainData.tsv";
    } else {
        ruta_archivo = "archivos/testData.tsv";
    }

    ifstream archivo;
    archivo.open(ruta_archivo);
    if (archivo.fail()) {
        cout << "ERROR! No se pudo encontrar el archivo." << endl;
    }

    getline(archivo, id, '\t' );

    clasificacionDelReview = "1";
    if (entrenando) {
        getline (archivo, clasificacionDelReview, '\t' );
    }

    getline (archivo, review, '\n' );

    int contador = 0;
    bool eliminar_StopWords = false;
    while (contador < FILTRO) {
        bool esPositivo = false;
        getline(archivo, id,'\t' );
        if (archivo.eof()) break;
        if (entrenando) {
            getline (archivo, clasificacionDelReview, '\t' );
            if (clasificacionDelReview == "1") esPositivo = true;
        }

        getline(archivo, review, '\n' );

        string review_filtrado = filtrar_review(&review, eliminar_StopWords);
        vector<string> lista_palabras = split(review_filtrado, ' ');
        vector<string> listaReview = construirNGrama(&lista_palabras, 1);
        vector<string> listaReview_bigrama = construirNGrama(&lista_palabras, 2);
        vector<string> listaReview_trigrama = construirNGrama(&lista_palabras, 3);

        unir_listas(&listaReview, &listaReview_bigrama);
        unir_listas(&listaReview, &listaReview_trigrama);

        cuerpoConLista unCuerpo;
        unCuerpo.sentiment = esPositivo;

        vector<int> hashingYValor(2, 0);

        for (vector<string>::iterator iterador=listaReview.begin(); iterador!=listaReview.end(); ++iterador){
            returnValue = hash64(*iterador, dimensiones);
            hashingYValor[0] = returnValue;
            hashingYValor[1] = 1;
            (unCuerpo.features).push_back(hashingYValor);
        }


        diccionario[id] = unCuerpo;
        contador++;
    }
    archivo.close();
    return diccionario;
}
