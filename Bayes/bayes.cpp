#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <sstream>

#define FILTRO 50

using namespace std;

set<string> crearListaDeStopWords();
bool esStopWord(string palabra, set<string> &listaStopsWords);
void contarReviews(float &reviewsTotales, float &reviewsPositivos, float &reviewsNegativos);
void crearDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN, set<string> &listaStopsWords);
string filtrarReview(string &review, set<string> &listaStopWords);
vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
vector<string> construirNGrama(vector<string> &listaPalabras, int tamanioNgrama);
void agregarADiccionarios(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN, vector<string> word, bool esPositivo, bool esNegativo);
void agregarADiccionario(map<string, float> &diccionario, vector<string> word);
void filtrarDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN);
void filtrarDiccionario(map<string, float> &diccionario);
void mostrarDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN);
void calcularProbabilidadesDePalabras(map<string, float> &diccionarioT, map<string, float> &diccionario, map<string, float> &probabilidades);
void clasificarReviewBayes(map<string, float> &probabilidadesP, map<string, float> &probabilidadesN, float reviewsTotales, float reviewsPositivos, float reviewsNegativos, set<string> &listaStopWords);
void mostrarResultadosPorPantalla(map<string, double> reviewsClasificadosPositivos, map<string, double> reviewsClasificadosNegativos, map<string, double> reviewsSinClasificar);
void unir_listas(vector<string> &lista_1, vector<string> &lista_2);

int main()
{
    map<string, float> diccionarioT, diccionarioP, diccionarioN, probabilidadesP, probabilidadesN;
    float reviewsTotales, reviewsPositivos, reviewsNegativos;
    set<string> listaStopWords;

    listaStopWords = crearListaDeStopWords();
    cout << "Contando cantidad de reviews positivos y negativos..." << endl;
    contarReviews(reviewsTotales, reviewsPositivos, reviewsNegativos);
    cout << "Inicializando diccionarios de palabras de los reviews..." << endl;
    crearDiccionariosDeReviews(diccionarioT, diccionarioP, diccionarioN, listaStopWords);
    cout << "Calculando Probabilidades Positivas" << endl;
    calcularProbabilidadesDePalabras(diccionarioT, diccionarioP, probabilidadesP);
    cout << "Calculando Probabilidades Negativas" << endl;
    calcularProbabilidadesDePalabras(diccionarioT, diccionarioN, probabilidadesN);
    cout << "En probabilidadesP hay  " << probabilidadesP.size() << endl;
    cout << "En probabilidadesN hay  " << probabilidadesN.size() << endl;
    cout << "Clasificando reviews..." << endl;
    clasificarReviewBayes(probabilidadesP, probabilidadesN, reviewsTotales, reviewsPositivos, reviewsNegativos, listaStopWords);

    return 0;
}

/* Cuenta la cantidad de reviews positivos y negativos */
void contarReviews(float &reviewsTotales, float &reviewsPositivos, float &reviewsNegativos)
{
    ifstream archivo;
    string clasificacionDelReview, id;
    archivo.open("archivos/labeledTrainData.tsv");

    if(archivo.fail())
    {
        cout << "Error al abrir el archivo labeledTrainData.tsv" << endl;
    }

    reviewsTotales = 0;
    reviewsNegativos = 0;
    reviewsPositivos = 0;

    while(true)
    {
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, clasificacionDelReview, '\t' );
        if (archivo.eof()) break;
        if(clasificacionDelReview == "1")
        {
            reviewsPositivos ++;
        }
        if(clasificacionDelReview == "0")
        {
            reviewsNegativos ++;
        }
    }

    archivo.close();

    reviewsTotales = reviewsPositivos + reviewsNegativos;

    cout << "En total hay  " << reviewsTotales << " reviews totales" << endl;
    cout << "En total hay  " << reviewsPositivos << " reviews positivos" << endl;
    cout << "En total hay  " << reviewsNegativos << " reviews negativos" << endl;
}

/*Inicializa 3 diccionarios:
- DiccionarioT : Contiene todas las palabras que aparecen en los reviews con sus cantidades
- DiccionarioP : Contiene todas las palabras que aparecen en los reviews positivos con sus cantidades
- DiccionarioN : Contiene todas las palabras que aparecen en los reviews negativos con sus cantidades
*/
void crearDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN, set<string> &listaStopsWords)
{
    ifstream archivo;
    string word, id, clasificacionDelReview, review;
    bool esPositivo, esNegativo;
    vector<string> listaPalabras, listaReview, listaReviewBigrama, listaReviewTrigrama;

    archivo.open("archivos/labeledTrainData.tsv");
    if(archivo.fail())
    {
        cout << "Error al abrir el archivo labeledTrainData.tsv" << endl;
    }

    while(true)
    {
        esPositivo = false;
        esNegativo = false;
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, clasificacionDelReview, '\t' ); // Lee la clasficacion hasta el TAB
        if (archivo.eof()) break;
        if (clasificacionDelReview == "1")
        {
            esPositivo = true;
        }
        if (clasificacionDelReview == "0")
        {
            esNegativo = true;
        }
        getline(archivo, review, '\n' ); // lee hasta el proximo tab
        review = filtrarReview(review, listaStopsWords);
        listaPalabras = split(review, ' ');
        listaReview = construirNGrama(listaPalabras, 1);
        listaReviewBigrama = construirNGrama(listaPalabras, 2);
        listaReviewTrigrama = construirNGrama(listaPalabras, 3);
        agregarADiccionarios(diccionarioT, diccionarioP, diccionarioN, listaReview, esPositivo, esNegativo);
        agregarADiccionarios(diccionarioT, diccionarioP, diccionarioN, listaReviewBigrama, esPositivo, esNegativo);
        agregarADiccionarios(diccionarioT, diccionarioP, diccionarioN, listaReviewTrigrama, esPositivo, esNegativo);
    }
    archivo.close();
 }

string filtrarReview(string &review, set<string> &listaStopWords)
{
    string cadena = "";
    string reviewFiltrado = "";
    bool eliminarStopWords = false;

    if ((listaStopWords).empty())
    {
        eliminarStopWords = false;
    }

    for (unsigned int indice = 0; indice < (review).size(); indice++)
    {
        char caracter = tolower((review)[indice]);
        if (!isalpha(caracter) && !isdigit(caracter))
        {
            if (cadena != "")
            {
                if (eliminarStopWords)
                {
                    if (!esStopWord(cadena, listaStopWords))
                    {
                        reviewFiltrado += cadena + " ";
                    }
                }
                else
                {
                    reviewFiltrado += cadena + " ";
                }
                cadena = "";
            }
        }
        else
        {
            cadena += caracter;
        }
    }
    return reviewFiltrado;
}

vector<string> &split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

vector<string> construirNGrama(vector<string> &listaPalabras, int tamanioNgrama)
{
    int cantPalabras = (listaPalabras).size();
    vector<string> listaNGramas;

    for (int indice = 0; indice <= (cantPalabras - tamanioNgrama); indice++)
    {
            string nGrama = "";
            for (int subIndice = 0; subIndice < tamanioNgrama; subIndice++)
            {
                nGrama += " " + ((listaPalabras)[(indice + subIndice)]);
            }
            listaNGramas.push_back(nGrama);
    }
    return listaNGramas;
}

void agregarADiccionarios(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN, vector<string> word, bool esPositivo, bool esNegativo)
{
    agregarADiccionario(diccionarioT, word);
    if (esPositivo)
    {
        agregarADiccionario(diccionarioP, word);
    }
    if (esNegativo)
    {
        agregarADiccionario(diccionarioN, word);
    }
 }

 void agregarADiccionario(map<string, float> &diccionario, vector<string> listaNGramas)
 {
    float cant;
    string palabra;

    for (unsigned int i = 0; i < listaNGramas.size() ; i++)
    {
        palabra = listaNGramas[i];
        map<string, float>::iterator iterador = diccionario.find(palabra);
        if(iterador != diccionario.end())
        {
            cant = iterador->second + 1;
            diccionario[palabra] = cant;
        }
        else
        {
            diccionario.insert( pair<string, float> (palabra, 1));
        }
    }
 }

 /* Filtra de los diccionarios aquellas palabras que aparezcan menos de FILTRO veces */
void filtrarDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN)
{
     filtrarDiccionario(diccionarioT);
     filtrarDiccionario(diccionarioP);
     filtrarDiccionario(diccionarioN);
}

/* Filtra un diccionario aquellas palabras que aparezcan menos de FILTRO veces */
void filtrarDiccionario(map<string, float> &diccionario)
{
    map<string, float>::iterator iterador;

     iterador = diccionario.begin();
     while (iterador != diccionario.end() )
     {
         if((iterador->second) < FILTRO)
         {
             diccionario.erase(iterador);
         }
         iterador ++;
     }
}

 /* Muestra por pantalla los diccionarios con las palabras y sus cantidades */
 void mostrarDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN)
 {
     //Muestra el diccionario completo por pantalla
    map<string, float>::iterator iterador = diccionarioT.begin();
    while (iterador != diccionarioT.end() )
    {
        cout << iterador->first << "      " <<iterador->second << endl;
        iterador ++;
    }

    cout << "En total se encontraron: " << diccionarioT.size() <<" palabras distintas"<< endl;
    cout << "En total se encontraron: " << diccionarioP.size() <<" palabras positivas"<< endl;
    cout << "En total se encontraron: " << diccionarioN.size() <<" palabras negativas"<< endl;
 }

set<string> crearListaDeStopWords()
{
    set<string> listaStopsWords;
    ifstream archivo;
    archivo.open("archivos/StopWords.txt");
    string palabra;
    if (archivo.fail()) {
        cout << endl << "Error al abrir el archivo de StopsWords" << endl;
    } else {
        while(true){
            getline(archivo, palabra,'\n' );
            if (archivo.eof())
            {
                break;
            }
            if (palabra != "") listaStopsWords.insert(palabra);
        }
    }
    return listaStopsWords;
}

/* Retorna true si la palabra es un stopWord, caso contrario retorna false */
bool esStopWord(string palabra, set<string> &listaStopWords)
{
    bool esStopWord;
    esStopWord = false;
    set<string>::iterator iterador = listaStopWords.find(palabra);
    if(iterador != listaStopWords.end())
    {
        esStopWord = true;
        return esStopWord;
    }
    return esStopWord;
}

void unir_listas(vector<string> &lista_1, vector<string> &lista_2)
{
    int tamanio_2 = (lista_2).size();
    for (int indice = 0; indice < tamanio_2; indice++) {
        (lista_1).push_back((lista_2)[indice]);
    }
}

/* Calcula las probabilidades de todas las palabras positivas */
void calcularProbabilidadesDePalabras(map<string, float> &diccionarioT, map<string, float> &diccionario, map<string, float> &probabilidades)
{
    float palabrasTotales, palabrasACalcularProbabilidad, cantidad, total, promedio;
    string word;

    palabrasTotales = diccionarioT.size();
    palabrasACalcularProbabilidad = 0;

    map<string, float>::iterator iterador = diccionario.begin();
    while (iterador != diccionario.end() )
    {
        palabrasACalcularProbabilidad += iterador->second;
        iterador ++;
    }

    total = palabrasTotales + palabrasACalcularProbabilidad;

    iterador = diccionarioT.begin();
    while (iterador != diccionarioT.end())
    {
        word = iterador->first;
        map<string, float>::iterator iteradorP = diccionario.find(word);
        if(iteradorP != diccionario.end())
        {
            cantidad = iteradorP->second + 1;
            promedio = (cantidad/total);
            probabilidades.insert(pair<string, float> (word, promedio));
        }
        else
        {
            promedio = (1/total);
            probabilidades.insert(pair<string, float> (word, promedio));
        }
        iterador ++;
    }
}

/*Realiza la clasificacion correspondiente de un archivo con reviews */
void clasificarReviewBayes(map<string, float> &probabilidadesP, map<string, float> &probabilidadesN, float reviewsTotales, float reviewsPositivos, float reviewsNegativos, set<string> &listaStopWords)
{
    ifstream archivo;
    ofstream archivoSalida;
    string word, id, review, numeroID;
    double probabilidadReviewP, probabilidadReviewN, probabilidadDeSerPositivo, probabilidadDeSerNegativo, probabilidadTotal, probabilidadBayes;
    map<string, double> reviewsClasificadosPositivos;
    map<string, double> reviewsClasificadosNegativos;
    map<string, double> reviewsSinClasificar;
    vector<string> listaPalabras;
    vector<string> listaReview;
    vector<string> listaReviewBigrama;
    vector<string> listaReviewTrigrama;

    probabilidadReviewP = (reviewsPositivos/reviewsTotales);
    probabilidadReviewN = (reviewsNegativos/reviewsTotales);

    archivo.open("archivos/testData.tsv");

    if(archivo.fail())
    {
        cout << "Error al abrir el archivo testData.tsv" << endl;
    }

    archivoSalida.open("archivos/resultadosBayes.csv");

    if(archivoSalida.fail())
    {
        cout << "Error al abrir el archivo resultadosBayes.csv" << endl;
    }

    archivoSalida << "\"id\",\"sentiment\"" << endl;

    while(true)
    {
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        numeroID = id;
        getline(archivo, review, '\n' ); // Lee el review
        if (archivo.eof()) break;
        probabilidadDeSerPositivo = probabilidadReviewP;
        probabilidadDeSerNegativo = probabilidadReviewN;
        review = filtrarReview(review, listaStopWords);
        listaPalabras = split(review, ' ');
        listaReview = construirNGrama(listaPalabras, 1);
        listaReviewBigrama = construirNGrama(listaPalabras, 2);
        listaReviewTrigrama = construirNGrama(listaPalabras, 3);

        unir_listas(listaReview, listaReviewBigrama);
        unir_listas(listaReview, listaReviewTrigrama);

        for (unsigned int i = 0; i < listaReview.size(); i++)
        {
            word = listaReview[i];
            // Busco si aparece esa palabra en las probabilidades Positivas y sumo el logaritmo de su probabilidad
            map<string, float>::iterator iteradorP = probabilidadesP.find(word);
            if(iteradorP != probabilidadesP.end())
            {
                probabilidadDeSerPositivo += log(iteradorP->second);
            }

            // Busco si aparece esa palabra en las probabilidades Negativas y sumo el logaritmo de su probabilidad
            map<string, float>::iterator iteradorN = probabilidadesN.find(word);
            if(iteradorN != probabilidadesN.end())
            {
                probabilidadDeSerNegativo += log(iteradorN->second);
            }
        }
        probabilidadTotal = (probabilidadDeSerPositivo + probabilidadDeSerNegativo);
        //Verifico si el review es Positivo o Negativo
        if((probabilidadDeSerPositivo != 0) && (probabilidadDeSerNegativo !=0))
        {
            if (probabilidadDeSerPositivo > probabilidadDeSerNegativo)
            {
                reviewsClasificadosPositivos.insert( pair<string, double> (numeroID, probabilidadDeSerPositivo));
                probabilidadBayes = 1 - (probabilidadDeSerPositivo/probabilidadTotal);
                archivoSalida << numeroID << ',' << probabilidadBayes << endl;
            }
            if (probabilidadDeSerPositivo < probabilidadDeSerNegativo)
            {
                reviewsClasificadosNegativos.insert( pair<string, double> (numeroID, probabilidadDeSerNegativo));
                probabilidadBayes = 1 - (probabilidadDeSerPositivo/probabilidadTotal);
                archivoSalida << numeroID << ',' << probabilidadBayes << endl;
            }
        }
        else
        {
            reviewsSinClasificar.insert( pair<string, double> (numeroID, 0));
        }
    }

    archivo.close();
    archivoSalida.close();

    mostrarResultadosPorPantalla(reviewsClasificadosPositivos, reviewsClasificadosNegativos, reviewsSinClasificar);
}

void mostrarResultadosPorPantalla(map<string, double> reviewsClasificadosPositivos, map<string, double> reviewsClasificadosNegativos, map<string, double> reviewsSinClasificar)
{
    map<string, double>::iterator iteradorReviewsP = reviewsClasificadosPositivos.begin();
    while (iteradorReviewsP != reviewsClasificadosPositivos.end())
    {
        cout << "La pelicula con ID:  " << iteradorReviewsP->first << " ha sido clasificada Positiva" << endl;
        iteradorReviewsP ++;
    }

    map<string, double>::iterator iteradorReviewsN = reviewsClasificadosNegativos.begin();
    while (iteradorReviewsN != reviewsClasificadosNegativos.end())
    {
        cout << "La pelicula con ID:  " << iteradorReviewsN->first << " ha sido clasificada Negativa" << endl;
        iteradorReviewsN ++;
    }

    cout << "Se han encontrado: " << reviewsClasificadosPositivos.size() << " reviews positivos" << endl;
    cout << "Se han encontrado: " << reviewsClasificadosNegativos.size() << " reviews negativos" << endl;
    cout << "Se han encontrado: " << reviewsSinClasificar.size() << " reviews sin clasificar" << endl;
    cout << "El total de reviews clasificados fue: " << reviewsClasificadosPositivos.size() + reviewsClasificadosNegativos.size() + reviewsSinClasificar.size() << endl;
}


