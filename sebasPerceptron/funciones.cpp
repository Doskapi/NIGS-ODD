
//Inclusiones necesarias
#include "funciones.h"

//Constantes para las funciones hash
#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U
#define FNV_PRIME_64 1099511628211
#define FNV_OFFSET_64 14695981039346656037U

#define DELTA_DE_APRENDIZAJE 0.1
#define TAMANIO_DE_LA_TABLA 100000 //33554432
#define FILTRO 50005

using namespace std;

//utiliza FNV32 para hashear un string y normaliza a la tabla de hash
int hash32(string &key, unsigned long int &tableSize) {
    int index;
    index = FNV32(key);
    index = index%tableSize;
    return index;
}

uint32_t FNV32(string &s) {
    // Hash FNV 32
    int a = s.size();
    uint32_t hash = FNV_OFFSET_32;
    for(int i = 0; i < a; i++)
    {
        hash = hash ^ (s[i]);
        hash = hash * FNV_PRIME_32;
    }
    return hash;
}

//utiliza FNV64 para hashear un string y normaliza a la tabla de hash
int hash64(string &key, unsigned long int &tableSize) {
    int index;
    index = FNV64(key);
    index = index%tableSize;
    return index;
}

uint64_t FNV64(string &s) {
    // Hash FNV 64
    int a = s.size();
    uint64_t hash = FNV_OFFSET_64;
    for(int i = 0; i < a; i++) {
        hash = hash ^ (s[i]);
        hash = hash * FNV_PRIME_64;
    }
    return hash;
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

//Dado una frase cualquiera y el tamanio del n grama devuleve
list <string>  construirNgrama(string &frase, int &tamanioNgrama) {
    list <string> listaNgramas;
    int cantidadPalabras = contarPalabras(frase);
    if (cantidadPalabras > tamanioNgrama) {
        stringstream unStream(frase);
        string palabra1, palabra2, unNgrama;
        int contadorNgrama = 0;
        while (getline(unStream, palabra1, ' ')) {
            unNgrama = "";
            palabra2 = palabra1;
            stringstream streamSegundario(frase);
            for(int i= 0; i< contadorNgrama; i++){
                getline(streamSegundario, palabra2, ' ');
            }
            for(int i= 0; i< tamanioNgrama; i++){
                getline(streamSegundario, palabra2, ' ');
                if (i < tamanioNgrama) unNgrama += palabra2 + " ";
                else unNgrama += palabra2;
            }
            if ((contadorNgrama + tamanioNgrama ) <= cantidadPalabras){
                    listaNgramas.push_back(unNgrama);
            }
            contadorNgrama++;
        }
    } else listaNgramas.push_front(frase);
	return listaNgramas;
}

//Retorna true si la palabra es un stopWord, caso contrario retorna false
bool buscarStopWord(string &word, list<string> & listaStopWords ) {
    list<string>::iterator iterador;
    for (iterador = listaStopWords.begin(); iterador != listaStopWords.end(); iterador++ ){
        if (*iterador == word) return true;
    }
    return false;
}

void incrementar(list<unsigned short int> & hashTable,unsigned long int &posicion ) {
    unsigned long int k = 0;
    list<unsigned short int>::iterator iter;
    for (iter = hashTable.begin();(k < posicion); iter++ ) k++;
    *iter = *iter + 1;
}

list<double> calcularPesos(map<string, cuerpoConHash> & diccionario, int &pasosMaximos){
    unsigned long int tableSize = TAMANIO_DE_LA_TABLA;
    list<double> listaDePesos (tableSize);
    list<double>::iterator itPesos;
    list<unsigned short int>::iterator itReviews;
    int errores = 0, error, positivo;
    unsigned long int i;
    double producto;
    for(int pasos=0; pasos < pasosMaximos; pasos ++){
        for (map<string,cuerpoConHash>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
            list<unsigned short int> reviews = it->second.hashTable;
            positivo = 0;
			producto = productoEscalar(reviews, listaDePesos);
            if (producto > 0.5) positivo = 1;
			error = (int)(it->second.sentiment) - positivo;
			if (error != 0){                //Actualizo los pesos
				errores += 1;
				itPesos = listaDePesos.begin();
				itReviews = reviews.begin();
				for(i= 0; i< tableSize; i++ ){
					*itPesos += DELTA_DE_APRENDIZAJE * error * log(1.+ *itReviews);
					itPesos++;
					itReviews++;
				}
			}
        }
        if (errores == 0){
			cout << "Cantidad de errores encontrados " << errores << endl;
            return listaDePesos;
        }
        errores=0;
        pasos++;
    }
    return listaDePesos;
}

map<string,cuerpoConHash> hashearNgramas(map<string,cuerpoConNgramas> &diccionario) {
    unsigned long int tableSize = TAMANIO_DE_LA_TABLA;
    //string idAnterior = "";
    map<string, cuerpoConHash> mapaFinal;
    cuerpoConHash unCuerpo;
    unsigned long int returnValue;
    for (map<string,cuerpoConNgramas>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
        //diccionario.erase(idAnterior);
        list <unsigned short int> hashTable(tableSize);
        unCuerpo.sentiment = it->second.sentiment;
        for (list<string>::iterator iterador=it->second.listaReview.begin(); iterador!=it->second.listaReview.end(); ++iterador){
            returnValue = hash32(*iterador, tableSize);
            incrementar(hashTable, returnValue);
        }
        //idAnterior = it ->first;
        unCuerpo.hashTable = hashTable;
        mapaFinal[it->first] = unCuerpo;
    }
    return mapaFinal;
}

double productoEscalar(list<unsigned short int> &review ,list<double> &listaDePesos) {
    double prod = 0;
    list<unsigned short int>::iterator itReview = review.begin();
    list<double>::iterator itPesos = listaDePesos.begin();
    for(unsigned long int i = 0; i < TAMANIO_DE_LA_TABLA; i++){
        prod += (*itPesos) * ((double) *itReview) ;
        itPesos++;
        itReview++;
    }
    return prod;
}

list <string>  crearListaDeStopWords() {

    list <string> listaStopsWords;
    ifstream archivo;
    archivo.open("archivos/StopWords.txt");
    string palabra;
    if(archivo.fail())
    {
        cout << endl<< "Error al abrir el archivo de StopsWords" << endl << endl;
    } else {
        while(!archivo.eof()){
            getline(archivo, palabra,'\n' );  // Lee las palabras hasta el fin de linea
            if (palabra != "") listaStopsWords.push_back(palabra);
        }
    }
	return listaStopsWords;
}

bool esLetra (char &c) {
     if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' ||
         c == 'i' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' ||
         c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' ||
         c == 'y' || c == 'z')
     {
         return true;
     }
     else
    {
        return false;
    }
 }

map<string, cuerpoConNgramas> obtenerNgramas(map<string, cuerpoDelReview> &diccionario, int &tamanio) {
    map<string, cuerpoConNgramas> mapaFinal;
    cuerpoConNgramas unCuerpo;
    for (map<string,cuerpoDelReview>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
        unCuerpo.sentiment = it->second.sentiment;
        unCuerpo.listaReview = construirNgrama(it->second.review,tamanio);
        mapaFinal[it->first] = unCuerpo;
        list<string> lista = mapaFinal[it->first].listaReview;
        /*for (list<string>::iterator iterador = lista.begin(); iterador != lista.end(); iterador++ ){
            cout << *iteradocuerpoDelReview unCuerpo;
        unCuerpo.sentiment = esPositivo;
        unCuerpo.review = reviewLimpio;
        diccionario[id] = unCuerpo;
r << endl;
        }*/
    }
    return mapaFinal;
}

map<string, cuerpoConNgramas> crearDiccionariosDeReviewsPerceptron(int &tamanio) {
    map<string, cuerpoDelReview> diccionario;
    crearDiccionarioInicial(diccionario);
    return obtenerNgramas(diccionario,tamanio);
}

void crearDiccionarioInicial(map<string, cuerpoDelReview> &diccionario) {
    list<string> listaDeStopWords = crearListaDeStopWords();
    ifstream archivo;
    string word, id, clasificacionDelReview, review;
    char c;
    bool esStopWord, esPositivo;
    archivo.open("archivos/labeledTrainData.tsv");

    if(archivo.fail())
    {
        cout << "Error, no se pudo encontrar el archivo en: archivos/labeledTrainData.tsv" << endl;
    }
    getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
    getline(archivo, clasificacionDelReview, '\t' );
    getline(archivo, review, '\n' );
    int contador = 0;
    while(!archivo.eof() & (contador < FILTRO))
    {
        esPositivo = false;
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, clasificacionDelReview, '\t' ); // Lee la clasficacion hasta el TAB
        if (clasificacionDelReview == "1")
        {
            esPositivo = true;
        }
        getline(archivo, review, '\n' ); // lee hasta el proximo tab
        string reviewFiltrado = "";
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
        cuerpoDelReview unCuerpo;
        unCuerpo.sentiment = esPositivo;
        unCuerpo.review = reviewFiltrado;
        diccionario[id] = unCuerpo;
        contador++;
    }
    archivo.close();
}

map<string, list<string> > crearDiccionariosDeReviewsDelArchAClasificar(int &tamanio) {
    map<string, string> diccionario;
    crearDiccionarioInicialDelArchAClasificar(diccionario);
    return obtenerNgramasDelArchAClasificar(diccionario,tamanio);
}

void crearDiccionarioInicialDelArchAClasificar(map<string, string> &diccionario) {
    list<string> listaDeStopWords = crearListaDeStopWords();
    ifstream archivo;
    string word, id, review;
    bool esStopWord;
    char c;
    archivo.open("archivos/testData.tsv");

    if(archivo.fail())
    {
        cout << "Error, no se pudo encontrar el archivo en: archivos/unlabeledTrainData.tsv" << endl;
    }
    getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
    getline(archivo, review, '\n' );
    int contador = 0;
    while(!archivo.eof() & (contador < FILTRO))
    {
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, review, '\n' ); // lee hasta el proximo tab
        string reviewFiltrado = "";
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

        diccionario[id] = reviewFiltrado;
        contador++;
    }
    archivo.close();
 }

map<string, list<string> > obtenerNgramasDelArchAClasificar(map<string, string> &diccionario, int &tamanio) {
    map<string, list<string> > mapaFinal;
    list<string> listaReview;
    for (map<string,string>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
        listaReview = construirNgrama(it->second,tamanio);
        mapaFinal[it->first] = listaReview;
    }
    return mapaFinal;
}


map<string, list<unsigned short int> > hashearNgramasDelArchAClasificar(map<string, list<string> >& diccionario) {
    unsigned long int tableSize = TAMANIO_DE_LA_TABLA;
    map<string, list<unsigned short int> > mapaFinal;
    unsigned long int returnValue;
    for (map<string, list<string> >::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
        list <unsigned short int> hashTable(tableSize);
        for (list<string>::iterator iterador=it->second.begin(); iterador!=it->second.end(); ++iterador){
            returnValue = hash32(*iterador, tableSize);
            incrementar(hashTable, returnValue);
        }
        mapaFinal[it->first] = hashTable;
    }
    return mapaFinal;
}
