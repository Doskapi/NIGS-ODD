#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <vector>
using namespace std;

void limpiarBasura(string&, string);

int main(){
    string line, id, clasificacionDelReview, review, palabraABuscar;
    long int cantidadDeReviews, cantTotalDePalabras, cantEncontradas;
    ifstream file;
    file.open("labeledTrainDataPosta.tsv");

    getline(file,line, '\n');
    cout << "Lector del data set" << endl;

    cout << "que palabra quiere buscar?" << endl;
    getline(cin, palabraABuscar);
    cantidadDeReviews = 0;
    cantTotalDePalabras = 0;
    cantEncontradas = 0;
    while (!file.eof()){
        getline(file, id, '\t' ); // lee el id del hasta el proximo tab
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
    return 0;
}

void limpiarBasura(string &strConBasura, string strALimpiar){
    if(strConBasura.find(strALimpiar) < strConBasura.length()){
        strConBasura.replace(strConBasura.find(strALimpiar),strALimpiar.length(),"");
        if(strConBasura.find(strALimpiar) < strConBasura.length()){
            limpiarBasura(strConBasura, strALimpiar);
        }
    }
}

