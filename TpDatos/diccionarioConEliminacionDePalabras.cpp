#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <string>

using namespace std;

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

int main()
{
    ifstream archivo;
    string word, line;
    char c;
    map<string, int> diccionario;
    bool esStopWord;
    int cant;

    archivo.open("labeledTrainData.tsv");

    if(archivo.fail())
    {
          cout << "Error al abrir el archivo labeledTrainData.tsv" << endl;
    }

    while(!archivo.eof())
    {

        getline(archivo, line, '\n');

        for (unsigned int i = 0; i < line.size(); ++i)
        {
            c = line[i];
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
    return 0;
}
