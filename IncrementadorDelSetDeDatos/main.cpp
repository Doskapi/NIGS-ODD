#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>

#define ARCH_RESULTADOS_PERCEPTRON "archivos/ResultadosPerceptron.csv"
#define ARCH_RESULTADOS_NAIVEBAYES "archivos/ResultadosNaiveBayes.csv"
#define ARCH_CLASIFICADO "archivos/unlabeledTrainData.tsv"
#define ARCH_DE_ENTRENAMIENTO "archivos/labeledTrainData.tsv"
#define ARCH_RELOADED "archivos/nuevoArchivoDeEntrenamiento.tsv"



using namespace std;

map<string, float> convertir_archivo_a_diccionario(const char* ruta_archivo) {
    map<string, float> diccionario;

    ifstream archivo;
    archivo.open(ruta_archivo);
    if (archivo.fail()) {
        cout << "No se ha podido abrir el archivo de id-probabilidad!" << endl;
    }

    string id, probabilidad;

    getline(archivo, id, ',' );
    getline(archivo, probabilidad, '\n' );

    while (true) {
        getline(archivo, id, ',' );
        getline(archivo, probabilidad, '\n' );

        if (archivo.eof()) break;
        diccionario[id] = std::stof(probabilidad);
    }
    archivo.close();
    return diccionario;
}

//Realiza una busqueda binaria sobre el vector de reviews a agregar, si el id recibido
//debe ser agregado, entonces se devuelve la posicion en la que se ha guardado en el
//vector de reviews a agregar. Si el id no se encuentra, se devuelve -1.
int indice_review_a_agregar(string id, vector< pair<string, float> > reviews_a_agregar) {
    int izquierda = 0;
    int derecha = reviews_a_agregar.size() - 1;
    int punto_medio = (izquierda + derecha) / 2;

    while (izquierda <= derecha) {
        if (id == get<0>(reviews_a_agregar[punto_medio])) {
            return punto_medio;
        } else {
            if (id > get<0>(reviews_a_agregar[punto_medio])) {
                izquierda = punto_medio + 1;
            } else {
                derecha = punto_medio - 1;
            }
        }
        punto_medio = (izquierda + derecha) / 2;
    }
    return -1;
}


//Pre: Es necesario que tanto Perceptron como Naive-Bayes hayan clasificado el mismo archivo,
//y que los id's clasificados se hayan escrito en el mismo orden en los archivos de resultados.
//Post: Devuelve un vector de strings, donde cada elemento es el id del review que ha sido
//clasificado de igual manera por ambos algoritmos.
vector< pair <string, float> > reviews_a_incorporar_al_set(map<string, float> dic_perceptron, map<string, float> dic_naivebayes) {
    vector< pair <string,float> > reviews_a_incorporar;
    map<string, float>::iterator it_dic_naivebayes;

    map<string, float>::iterator it_dic_perceptron = dic_perceptron.begin();
    while (it_dic_perceptron != dic_perceptron.end()) {
        string id_perceptron = it_dic_perceptron->first;
        it_dic_naivebayes = dic_naivebayes.find(id_perceptron);
        float proba_perceptron = it_dic_perceptron->second;
        float proba_naivebayes = it_dic_naivebayes->second;
        bool ambos_positivos = proba_perceptron >= 0.5 && proba_naivebayes >= 0.5;
        bool ambos_negativos = proba_perceptron  < 0.5 && proba_naivebayes  < 0.5;

        if (ambos_positivos || ambos_negativos) {
            reviews_a_incorporar.push_back(pair <string, float> (id_perceptron, proba_perceptron));
        }

        ++it_dic_perceptron;
    }
    return reviews_a_incorporar;
}


void clonar_archivo_de_entrenamiento(const char* direccionDeArchAClonar, const char* direccionDeArchClonado){
    ifstream archivoAClonar;

    archivoAClonar.open(direccionDeArchAClonar);
    if (archivoAClonar.fail()) {
        cout << "ERROR! No se pudo encontrar el archivo a clonar." << endl;
    }

    ofstream archivoClonado;
    archivoClonado.open(direccionDeArchClonado);

    string dato;

    while(true){
        getline(archivoAClonar, dato, '\n' );
        if (archivoAClonar.eof()) break;
        archivoClonado << dato << endl;
    }
    archivoAClonar.close();
    archivoClonado.close();
}


void incorporar_reviews_al_set(const char* dir_archivo_reload, const char* dir_archivo_con_reviews, vector< pair<string,float> > reviews_a_incorporar) {
    ofstream archivo_reload;
    archivo_reload.open(dir_archivo_reload, ios::app);
    if (archivo_reload.fail()) {
        cout << "ERROR! No se pudo encontrar el archivo de entrenamiento!" << endl;
    }

    ifstream archivo_con_reviews;
    archivo_con_reviews.open(dir_archivo_con_reviews);
    if (archivo_con_reviews.fail()) {
        cout << "ERROR! No se pudo encontrar el archivo con reviews!" << endl;
    }

    string id, review;

    //Leo el encabezado.
    getline(archivo_con_reviews, id,'\t' );
    getline(archivo_con_reviews, review, '\n' );
    int contador = 0;
    while(true) {
        contador++;
        getline(archivo_con_reviews, id,'\t' );
        getline(archivo_con_reviews, review, '\n');
        if (archivo_con_reviews.eof()) break;

        int indice_review = indice_review_a_agregar(id, reviews_a_incorporar);
        cout << "EL indice fue " << indice_review << " para el review " << contador << endl;
        //Si el indice_review es mayor o igual a cero, entonces hay que agregarlo.
        if (indice_review >= 0) {
            float clasificacion = get<1>(reviews_a_incorporar[indice_review]);

            if (clasificacion >= 0.5) {
                archivo_reload << id << "\t" << 1 << "\t" << review << endl;
            } else {
                archivo_reload << id << "\t" << 0 << "\t" << review << endl;
            }

        }
    }

    archivo_con_reviews.close();
    archivo_reload.close();
}


int main() {
    map<string, float> dic_perceptron = convertir_archivo_a_diccionario(ARCH_RESULTADOS_PERCEPTRON);
    map<string, float> dic_naivebayes = convertir_archivo_a_diccionario(ARCH_RESULTADOS_NAIVEBAYES);
    vector< pair<string, float> > reviews = reviews_a_incorporar_al_set(dic_perceptron, dic_naivebayes);
    clonar_archivo_de_entrenamiento(ARCH_DE_ENTRENAMIENTO, ARCH_RELOADED);
    incorporar_reviews_al_set(ARCH_RELOADED, ARCH_CLASIFICADO, reviews);

    return 0;
}
