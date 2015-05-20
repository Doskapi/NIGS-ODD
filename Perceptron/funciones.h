
//Inclusiones necesarias
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <stdint.h>

using namespace std;


int contarPalabras(string frase);

list <string>  construirNgrama(string, int);

uint32_t FNV32(string s); 

int hash32(string key, unsigned long int tableSize);

uint64_t FNV64(string s);

int hash64(string key, unsigned long int tableSize);
 