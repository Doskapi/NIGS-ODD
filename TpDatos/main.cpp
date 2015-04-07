/************************************************************************************
 * Facultad de Ingeniería - Universidad de Buenos Aires
 * 75.06 - Organizacion de Datos
 * Cátedra Argerich
 * Tp Grupal - Grupo 1 - Araya Campoamor Goicoa Recalde
 *************************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

void limpiarBasura(string&, string);
void sacarBasuraDelArchivo();
bool buscarStopWord(string);
void crearDiccionarioDeReviews(map<string, int>&);
void crearDiccionarioDePalabrasConSuPuntaje(map<string, int>&);

int main(){
    map<string, int> dicDePalPuntaje, dicDeReviewYCantDeVecesQueApareceLaPalabra;

    cout << "Funciones utiles para el tp" << endl;
    cout << endl;

    sacarBasuraDelArchivo();

    cout << "Iniciando diccionario de palabras con su puntaje..." << endl;
    crearDiccionarioDePalabrasConSuPuntaje(dicDePalPuntaje);
    cout << "DONE" << endl;

    cout << "Iniciando diccionario de palabras en los reviews..." << endl;
    crearDiccionarioDeReviews(dicDeReviewYCantDeVecesQueApareceLaPalabra);
    cout << "DONE" << endl;

    return 0;
}

void sumarPuntajeDelReview()
{
         string line, id, clasificacionDelReview, review, palabraABuscar;
    long int cantidadDeReviews, cantTotalDePalabras, cantEncontradas;
    ifstream file;
    file.open("labeledTrainData.tsv");

    getline(file,line, '\n');
    cout << "Lector del data set" << endl;

    cout << "que palabra quiere buscar?" << endl;
    getline(cin, palabraABuscar);
    cantidadDeReviews = 0;
    cantTotalDePalabras = 0;
    cantEncontradas = 0;
    while (!file.eof()){
        getline(file, id,'\t' ); // lee el id del hasta el proximo tab
        if (!id.empty()){ //por si hay alguna linea vacia
            id = id.substr(1,id.length()-2);
            getline(file, clasificacionDelReview, '\t' ); // lee la clasif del review hasta el proximo tab
            getline(file, review, '\n' ); // lee hasta el proximo tab

            limpiarBasura(review,"<br />");
            limpiarBasura(review,"\\");
            limpiarBasura(review,"");

            cantidadDeReviews++;
            cantTotalDePalabras = cantTotalDePalabras + review.length();

            if (review.find(palabraABuscar) < review.length()){
                // cout << id << endl;
                cantEncontradas++;
                // cout << id << " pos:" << review.find(palabraABuscar) << endl;
            }
        }
    }

    cout << "Se encontraron " << cantEncontradas << " reviews con la palabra \"" << palabraABuscar << "\"" << endl;
    cout << "Promedio: " << cantTotalDePalabras << "/" << cantidadDeReviews << " = " << cantTotalDePalabras/cantidadDeReviews << endl;

    file.close();
}

void limpiarBasura(string &strConBasura, string strALimpiar)
{
    if(strConBasura.find(strALimpiar) < strConBasura.length()){
        strConBasura.replace(strConBasura.find(strALimpiar),strALimpiar.length(),"");
        if(strConBasura.find(strALimpiar) < strConBasura.length()){
            limpiarBasura(strConBasura, strALimpiar);
        }
    }
}

void sacarBasuraDelArchivo()
{
    string line, id, clasificacionDelReview, review, palabraABuscar;
    long int cantidadDeReviews, cantTotalDePalabras, cantEncontradas;
    ifstream file;
    file.open("labeledTrainData.tsv");

    getline(file,line, '\n');
    cout << "Lector del data set" << endl;

    cout << "que palabra quiere buscar?" << endl;
    getline(cin, palabraABuscar);
    cantidadDeReviews = 0;
    cantTotalDePalabras = 0;
    cantEncontradas = 0;
    while (!file.eof()){
        getline(file, id,'\t' ); // lee el id del hasta el proximo tab
        if (!id.empty()){ //por si hay alguna linea vacia
            id = id.substr(1,id.length()-2);
            getline(file, clasificacionDelReview, '\t' ); // lee la clasif del review hasta el proximo tab
            getline(file, review, '\n' ); // lee hasta el proximo tab

            limpiarBasura(review,"<br />");
            limpiarBasura(review,"\\");
            limpiarBasura(review,"");

            cantidadDeReviews++;
            cantTotalDePalabras = cantTotalDePalabras + review.length();

            if (review.find(palabraABuscar) < review.length()){
                // cout << id << endl;
                cantEncontradas++;
                // cout << id << " pos:" << review.find(palabraABuscar) << endl;
            }
        }
    }

    cout << "Se encontraron " << cantEncontradas << " reviews con la palabra \"" << palabraABuscar << "\"" << endl;
    cout << "Promedio: " << cantTotalDePalabras << "/" << cantidadDeReviews << " = " << cantTotalDePalabras/cantidadDeReviews << endl;

    file.close();
}

/*Retorna true si la palabra es un stopWord, caso contrario retorna false*/
bool buscarStopWord(string word)
{
    string w = word;
    if (w == "a" || w == "about" || w == "above" || w == "after" || w == "again" || w == "against" ||
        w == "all" || w == "am" || w == "an" || w == "and" || w == "any" || w == "are" || w == "aren't" ||
        w == "as" || w == "at" || w == "be" || w == "because" || w == "been" || w == "before" || w == "being" ||
        w == "bellow" || w == "between" || w == "both" || w == "but" ||w == "by" || w == "can't" || w == "cannot" ||
        w == "could" || w == "couldn't" || w == "did" || w == "didn't" || w == "do" || w == "does" ||
        w == "doesn't" || w == "doing" || w == "don't" || w == "down" || w == "during" || w == "each" ||
        w == "few" || w == "for" || w == "from" || w == "further" || w == "had" || w == "hadn't" ||
        w == "has" || w == "hasn\'t" || w == "have" || w == "haven't" || w == "having" || w == "he" ||
        w == "he'd" || w == "he\'ll" || w == "he's" || w == "her" || w == "here" || w == "here's" ||
        w == "hers" || w == "herself" || w == "him" || w == "himself" || w == "his" || w == "how" || w == "how's" ||
        w == "i" || w == "i'd" || w == "i'll" || w == "i'm" || w == "i've" || w == "if" || w == "in" ||
        w == "into" || w == "is" || w == "isn't" || w == "it's" || w == "its" || w == "itself" || w == "let's" ||
        w == "me" || w == "more" || w == "most" || w == "mustn't" || w == "my" || w == "myself" || w == "no" ||
        w == "nor" || w == "not" || w == "of" || w == "off" || w == "on" || w == "once" || w == "only" ||
        w == "or" || w == "other" || w == "ought" || w == "our" || w == "ours" || w == "ourselves" || w == "out" ||
        w == "over" || w == "own" || w == "same" || w == "shan't" || w == "she" || w == "she'd" || w == "she'll" ||
        w == "she's" || w == "should" || w == "shouldn't" || w == "so" || w == "some" || w == "such" || w == "than" ||
        w == "that" || w == "that's" || w == "the" || w == "their" || w == "theirs" || w == "them" || w == "themselves" ||
        w == "then" || w == "there" || w == "there's" || w == "these" || w == "they" || w == "they'd" ||
        w == "they'll" || w == "they're" || w == "they've" || w == "this" || w == "those" || w == "through" ||
        w == "to" || w == "too" || w == "under" || w == "until" || w == "up" || w == "very" || w == "was" ||
        w == "wasn't" || w == "we" || w == "we'd" || w == "we'll" || w == "we're" || w == "we've" ||
        w == "were" || w == "weren't" || w == "what" || w == "what's" || w == "when" || w == "when's" ||
        w == "where" || w == "where's" || w == "which" || w == "while" || w == "who" || w == "who's" || w == "whom" ||
        w == "why" || w == "why's" || w == "with" || w == "won't" || w == "would'" || w == "wouldn\'t" ||
        w == "you" || w == "you'd" || w == "you'll" || w == "you're" || w == "you've" || w == "your" ||
        w == "yours" || w == "yourself" || w == "yourselves")
    {
        return true;
    }
    else
    {
        return false;
    }
}

void crearDiccionarioDeReviews(map<string, int> &diccionario)
{
    ifstream archivo;
    string word, linea;
    char c;
    bool esStopWord;
    int cant;

    archivo.open("labeledTrainData.tsv");

    if(archivo.fail())
    {
        cout << "Error al abrir el archivo labeledTrainData.tsv" << endl;
    }

    while(!archivo.eof())
    {
        getline(archivo, linea, '\n');

        for (unsigned int i = 0; i < linea.size(); ++i)
        {
            c = linea[i];
            c = tolower(c);

            if(c == ',' || c == '.' || c == '-' || c == '"' || c == ' ' || c == '<' || c == '>' ||
               c == '/' || c == '\\'|| c == '(' || c == ')' || c == '?' || c == '!' || c == '.' ||
               c == '_' || c == '$' || c == '#' || c == '%' || c == '&' || c == '@' ||c == '_'  ||
               c == '\n'|| c == '\t'|| c == ':' || c == ';' || c == '0' || c == '1' || c == '2' ||
               c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
            {
                if(!word.empty())
                {
                    esStopWord = false;
                    esStopWord = buscarStopWord(word);
                    if(!esStopWord)
                    {
                        //Agrega al diccionario generico de todas las palabras
                        map<string, int>::iterator iterador = diccionario.find(word);
                        if(iterador != diccionario.end())
                        {
                            cant = iterador->second + 1;
                            diccionario[word] = cant;
                        }
                        else
                        {
                            diccionario.insert( pair<string, int> (word, 1));
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


    //Muestra el diccionario completo por pantalla
    map<string, int>::iterator it = diccionario.begin();
    while (it != diccionario.end() )
    {
        cout << it->first << "      " <<it->second << endl;
        it ++;
    }

    cout << "En total se encontraron: " << diccionario.size() <<" palabras distintas"<< endl;
    archivo.close();
 }

void crearDiccionarioDePalabrasConSuPuntaje(map<string, int> &diccionario)
{
    ifstream archivo;
    string palabra, puntaje;
    int puntajeComoInt;


    archivo.open("AFINN-111.tsv");

    if(archivo.fail())
    {
        cout << "Error al abrir el archivo AFINN-111.txt" << endl;
    }

    while(!archivo.eof())
    {
        getline(archivo, palabra, '\t');
        getline(archivo, puntaje, '\n');
        //Agrega al diccionario generico de todas las palabras
        map<string, int>::iterator iterador = diccionario.find(palabra);
        if(iterador != diccionario.end())
        {
            stringstream(puntaje) >> puntajeComoInt;
            diccionario[palabra] = puntajeComoInt;
        }
        else
        {
            stringstream(puntaje) >> puntajeComoInt;
            diccionario.insert( pair<string, int> (palabra, puntajeComoInt));
        }
    }

//    //Muestra el diccionario completo por pantalla
//    map<string, int>::iterator it = diccionario.begin();
//    while (it != diccionario.end() )
//    {
//        cout << it->first << "      " <<it->second << endl;
//        it ++;
//    }
//
    cout << "En total se encontraron: " << diccionario.size() <<" palabras distintas"<< endl;
    archivo.close();
}
