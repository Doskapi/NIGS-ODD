
//Inclusiones necesarias
#include "archivoTexto.h"

using namespace std;

ArchivoTexto::ArchivoTexto(const std::string& path) {
  //intenta abrir el archivo en modo lectura - escritura
  archivo.open(path.c_str(), std::fstream::in | std::fstream::out);

  if (!archivo.is_open()) {
    //si no hubo éxito en la apertura...
    //limpia los flags de control de estado del archivo
    archivo.clear();

    //crea el archivo
    archivo.open(path.c_str(), std::fstream::out);
    archivo.close();

    //reabre el archivo para lectura - escritura
    archivo.open(path.c_str(), std::fstream::in | std::fstream::out);

    if (!archivo.is_open())
      // si no se pudo crear el archivo arroja una excepción/
      throw std::ios_base::failure("El archivo no pudo ser abierto");
  }
}

ArchivoTexto::~ArchivoTexto() {
  archivo.close();
}

void ArchivoTexto::escribir(char c) {
  // Intenta escribir el caracter en el archivo
  archivo.put(c);

  if (archivo.fail())
    //si se produjo un error, arroja una excepción
    throw std::ios_base::failure("No se pudo escribir correctamente el registro");
}

void ArchivoTexto::escribir(const std::string& cadena) {
  //intenta escribir la cadena en el archivo
  archivo << cadena;

  if (archivo.fail())
    // si se produjo un error, arroja una excepción
    throw std::ios_base::failure("No se pudo escribir correctamente la cadena");
}

void ArchivoTexto::terminarLinea() {
  // intenta escribir en el archivo
  archivo << endl;

  if (archivo.fail())
    //si se produjo un error, arroja una excepción
    throw std::ios_base::failure("No se pudo terminar la linea");
}

void ArchivoTexto::tabular() {
  escribir('\t');
}

bool ArchivoTexto::leerLinea(std::string &cadena) {
  char linea[MAX_LENGTH];

  // lee del archivo a la linea, hasta haber leido:
  // MAX_LENGTH caracteres, o un fin de linea
  archivo.getline( (char*)&linea , MAX_LENGTH , '\n');
  cadena = linea;

  if (archivo.fail() ) {
    //chequea si se ha producido un error, se devuelve false
    archivo.clear();
    return false;
  }
  return true;
}



/**
 * Posiciona el cursor al comienzo del archivo
 */
void ArchivoTexto::irAlPrincipio() {
  archivo.tellg();
  archivo.clear();
  archivo.seekg(0);
  archivo.seekp(0);
  archivo.tellg();
}

/**
 * Posiciona el cursor al final del archivo
 */
void ArchivoTexto::irAlFinal() {
  archivo.tellg();
  archivo.clear();
  archivo.seekg(0, ios::end);
  archivo.seekp(0, ios::end);
  archivo.tellg();
}

