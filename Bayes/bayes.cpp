#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <string>
#include <cmath>

#define FILTRO 50

using namespace std;

bool esLetra (char c);
bool buscarStopWord(string word);
void contarReviews(float &reviewsTotales, float &reviewsPositivos, float &reviewsNegativos);
void crearDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN);
void filtrarDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN);
void filtrarDiccionario(map<string, float> &diccionario);
void mostrarDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN);
void calcularProbabilidadesDePalabras(map<string, float> &diccionarioT, map<string, float> &diccionario, map<string, float> &probabilidades);
void clasificarReviewBayes(map<string, float> &probabilidadesP, map<string, float> &probabilidadesN, float reviewsTotales, float reviewsPositivos, float reviewsNegativos);

int main()
{
    map<string, float> diccionarioT, diccionarioP, diccionarioN, probabilidadesP, probabilidadesN;
    float reviewsTotales, reviewsPositivos, reviewsNegativos;

    cout << "Contando cantidad de reviews positivos y negativos..." << endl;
    contarReviews(reviewsTotales, reviewsPositivos, reviewsNegativos);
    cout << "Inicializando diccionarios de palabras de los reviews..." << endl;
    crearDiccionariosDeReviews(diccionarioT, diccionarioP, diccionarioN);
    //cout << "Filtrando los diccionarios de palabras de los reviews..." << endl;
    //filtrarDiccionariosDeReviews(diccionarioT, diccionarioP, diccionarioN)
    //mostrarDiccionariosDeReviews(diccionarioT, diccionarioP, diccionarioN);
    cout << "Calculando Probabilidades Positivas" << endl;
    calcularProbabilidadesDePalabras(diccionarioT, diccionarioP, probabilidadesP);
    cout << "Calculando Probabilidades Negativas" << endl;
    calcularProbabilidadesDePalabras(diccionarioT, diccionarioN, probabilidadesN);
    cout << "En probabilidadesP hay  " << probabilidadesP.size() << endl;
    cout << "En probabilidadesN hay  " << probabilidadesN.size() << endl;
    cout << "Clasificando reviews..." << endl;
    clasificarReviewBayes(probabilidadesP, probabilidadesN, reviewsTotales, reviewsPositivos, reviewsNegativos);

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

    while(!archivo.eof())
    {
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        getline(archivo, clasificacionDelReview, '\t' );
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
void crearDiccionariosDeReviews(map<string, float> &diccionarioT, map<string, float> &diccionarioP, map<string, float> &diccionarioN)
{
    ifstream archivo;
    string word, id, clasificacionDelReview, review;
    char c;
    bool esStopWord, esPositivo, esNegativo, letra;
    float cant;

    archivo.open("labeledTrainData.tsv");

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
            letra = esLetra(c);

            if(!letra)
            {
                if(!word.empty())
                {
                    esStopWord = false;
                    esStopWord = buscarStopWord(word);
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

 /* Retorna true si el char es una letra, caso contrario retorna false */
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

/* Retorna true si la palabra es un stopWord, caso contrario retorna false */
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

//    Muestra por pantalla las palabras con su probabilidad
//    map<string, float>::iterator iteradorM = probabilidades.begin();
//    while (iteradorM != probabilidades.end())
//    {
//        cout << "La palabra  " << iteradorM->first << "  Tiene un promedio " << iteradorM->second << endl;
//        iteradorM ++;
//    }
}

/*Realiza la clasificacion correspondiente de un archivo con reviews */
void clasificarReviewBayes(map<string, float> &probabilidadesP, map<string, float> &probabilidadesN, float reviewsTotales, float reviewsPositivos, float reviewsNegativos)
{
    ifstream archivo;
    ofstream archivoSalida, archivoProbabilidades;
    string word, id, review, numeroID;
    char c;
    bool esStopWord, letra;
    double probabilidadReviewP, probabilidadReviewN, probabilidadDeSerPositivo, probabilidadDeSerNegativo, probabilidadTotal, probabilidadBayes;
    map<string, double> reviewsClasificadosPositivos;
    map<string, double> reviewsClasificadosNegativos;
    map<string, double> reviewsSinClasificar;

    probabilidadReviewP = (reviewsPositivos/reviewsTotales);
    probabilidadReviewN = (reviewsNegativos/reviewsTotales);

    archivo.open("archivos/testData.tsv");

    if(archivo.fail())
    {
        cout << "Error al abrir el archivo testData.tsv" << endl;
    }

    archivoSalida.open("salida.csv");

    if(archivoSalida.fail())
    {
        cout << "Error al abrir el archivo salida.csv" << endl;
    }

    archivoProbabilidades.open("probabilidadesBayes.csv");

    if(archivoProbabilidades.fail())
    {
        cout << "Error al abrir el archivo salida.csv" << endl;
    }

    while(!archivo.eof())
    {
        getline(archivo, id,'\t' );  // Lee el ID hasta el TAB
        numeroID = id;
        getline(archivo, review, '\n' ); // Lee el review
        probabilidadDeSerPositivo = probabilidadReviewP;
        probabilidadDeSerNegativo = probabilidadReviewN;
        for (unsigned int i = 0; i < review.size(); ++i)
        {
            c = review[i];
            c = tolower(c);
            letra = esLetra(c);

            if(!letra)
            {
                if(!word.empty())
                {
                    esStopWord = false;
                    esStopWord = buscarStopWord(word);
                    if(!esStopWord)
                    {
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
                        word.clear();
                    }
                }
            }
            else
            {
                word += c;
            }
        }

        probabilidadTotal = (probabilidadDeSerPositivo + probabilidadDeSerNegativo);
        //Verifico si el review es Positivo o Negativo
        if((probabilidadDeSerPositivo != 0) && (probabilidadDeSerNegativo !=0))
        {
            if (probabilidadDeSerPositivo > probabilidadDeSerNegativo)
            {
                reviewsClasificadosPositivos.insert( pair<string, double> (numeroID, probabilidadDeSerPositivo));
                archivoSalida << numeroID << ',' << "1" << endl;
                probabilidadBayes = 1 - (probabilidadDeSerPositivo/probabilidadTotal);
                archivoProbabilidades << numeroID << ',' << probabilidadBayes << ',' << "1" << endl;
            }
            if (probabilidadDeSerPositivo < probabilidadDeSerNegativo)
            {
                reviewsClasificadosNegativos.insert( pair<string, double> (numeroID, probabilidadDeSerNegativo));
                archivoSalida << numeroID << ',' << "0" << endl;
                probabilidadBayes = 1 - (probabilidadDeSerPositivo/probabilidadTotal);
                archivoProbabilidades << numeroID << ',' << probabilidadBayes << ',' << "0" << endl;
            }
        }
        else
        {
            reviewsSinClasificar.insert( pair<string, double> (numeroID, 0));
        }
    }

    archivo.close();
    archivoSalida.close();

    //Muestro los resultados por pantalla
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

    cout << "Se han encontrado: " << reviewsClasificadosPositivos.size() << "reviews positivos" << endl;
    cout << "Se han encontrado: " << reviewsClasificadosNegativos.size() << "reviews negativos" << endl;
    cout << "Se han encontrado: " << reviewsSinClasificar.size() << " reviews sin clasificar" << endl;
    cout << "El total de reviews clasificados fue: " << reviewsClasificadosPositivos.size() + reviewsClasificadosNegativos.size() + reviewsSinClasificar.size() << endl;
}


