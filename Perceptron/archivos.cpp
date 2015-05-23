
//Inclusiones necesarias
#include "archivos.h"
#include "funciones.h"

#define FILTRO 50

list <string>  crearListaDeStopWords(){

    list <string> listaStopsWords;
    ifstream archivo;
    archivo.open("archivos/StopWords.txt");
    string palabra;
    if(archivo.fail())
    {
        cout << endl<< "Error al abrir el archivo de StopsWords" << endl << endl;
    } else {
        cout << endl << "Archivo de StopsWords abierto correctamente" << endl << endl;
        while(!archivo.eof()){
            getline(archivo, palabra,'\n' );  // Lee las palabras hasta el fin de linea
            if (palabra != "") listaStopsWords.push_back(palabra);
        }
    }
	return listaStopsWords;
}

 bool esLetra (char c)
 {
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

void crearDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN)
{
    list<string> listaDeStopWords = crearListaDeStopWords();
    ifstream archivo;
    string word, id, clasificacionDelReview, review;
    char c;
    bool esStopWord, esPositivo, esNegativo;
    float cant;

    archivo.open("archivos/labeledTrainData.tsv");

    if(archivo.fail())
    {
        cout << "Error al abrir el archivo labeledTrainData.tsv" << endl;
    }

    while(!archivo.eof())
    {
        esPositivo = false;
        esNegativo = false;
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, clasificacionDelReview, '\t' ); // Lee la clasficacion hasta el TAB
        if (clasificacionDelReview == "1")
        {
            esPositivo = true;
        }
        if (clasificacionDelReview == "0")
        {
            esNegativo = true;
        }
        getline(archivo, review, '\n' ); // lee hasta el proximo tab

        for (unsigned int i = 0; i < review.size(); ++i)
        {
            c = review[i];
            c = tolower(c);

            if(!esLetra(c))
            {
                if(!word.empty())
                {
                    esStopWord = buscarStopWord(word, listaDeStopWords);
                    if(!esStopWord)
                    {
                        //Agrega al diccionario generico de todas las palabras
                        map<string, float>::iterator iterador = diccionarioT.find(word);
                        if(iterador != diccionarioT.end())
                        {
                            cant = iterador->second + 1;
                            diccionarioT[word] = cant;
                        }
                        else
                        {
                            diccionarioT.insert( pair<string, float> (word, 1));
                        }

                        if (esPositivo)
                        {
                            // Agrega al diccionario de palabras positivas
                             map<string, float>::iterator iterador = diccionarioP.find(word);
                            if(iterador != diccionarioP.end())
                            {
                                cant = iterador->second + 1;
                                diccionarioP[word] = cant;
                            }
                            else
                            {
                                diccionarioP.insert( pair<string, float> (word, 1));
                            }
                        }

                         if (esNegativo)
                        {
                            // Agrega al diccionario de palabras negativas
                             map<string, float>::iterator iterador = diccionarioN.find(word);
                            if(iterador != diccionarioN.end())
                            {
                                cant = iterador->second + 1;
                                diccionarioN[word] = cant;
                            }
                            else
                            {
                                diccionarioN.insert( pair<string, float> (word, 1));
                            }
                        }

                        word.clear();
                    }
                }
            }
            else
            {
                word += c;
            }
        }
    }
    archivo.close();
 }


void crearDiccionarioInicial(map<string, cuerpoDelReview> &diccionario)
{
    list<string> listaDeStopWords = crearListaDeStopWords();
    ifstream archivo;
    string word, id, clasificacionDelReview, review;
    char c;
    bool esStopWord, esPositivo;
    archivo.open("archivos/labeledTrainData.tsv");

    if(archivo.fail())
    {
        cout << "Error al abrir el archivo labeledTrainData.tsv" << endl;
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

map<string, cuerpoConNgramas> obtenerNgramas(map<string, cuerpoDelReview> &diccionario, int tamanio){
    map<string, cuerpoConNgramas> mapaFinal;
    cuerpoConNgramas unCuerpo;
    for (map<string,cuerpoDelReview>::iterator it=diccionario.begin(); it!=diccionario.end(); ++it){
        unCuerpo.sentiment = it->second.sentiment;
        unCuerpo.listaReview = construirNgrama(it->second.review,tamanio);
        mapaFinal[it->first] = unCuerpo;
        /*list<string> lista = mapaFinal[it->first].listaReview;
        for (list<string>::iterator iterador = lista.begin(); iterador != lista.end(); iterador++ ){
            cout << *iterador << endl;
        }*/
    }
    return mapaFinal;
}

map<string, cuerpoConNgramas> crearDiccionariosDeReviewsPerceptron(int tamanio) {
    map<string, cuerpoDelReview> diccionario;
    crearDiccionarioInicial(diccionario);
    return obtenerNgramas(diccionario,tamanio);
}
