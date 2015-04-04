//============================================================================
// Name        : TpDatos.cpp
// Author      : Nicolas Araya
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//Includes
#include <iostream>
#include "archivoTexto.h"
#include "Review.h"

using namespace std;

/*Imprime el id y el body de cada uno de los Reviews*/
void imprimirDatos(string linea){
	Review unReview(linea);
	cout << unReview.getId() << endl << unReview.getBody() << endl;
}

int main(int argc, char **argv) {
  ArchivoTexto miArchivo("labeledTrainData.tsv");
  string linea;
  miArchivo.irAlPrincipio();
  miArchivo.leerLinea(linea);
  int i = 0;
  Review unReview();
  while ((miArchivo.leerLinea(linea))&( i < 10)) {
	  imprimirDatos(linea);
	  i++;
  }
  return 0;
}





