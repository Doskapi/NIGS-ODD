#include <iostream>
#include <fstream>
#include <string>


using namespace std;

void incrementarSetDatos(bool esPositivo);

int main()
{
    incrementarSetDatos(true);
    incrementarSetDatos(false);
    return 0;
}

void incrementarSetDatos(bool esPositivo)
{
    string nombreArchivo;
    string label;
    if(esPositivo)
    {
        nombreArchivo = "archivos/pos/Positivo ";
        label = "1";
    }
    else
    {
        nombreArchivo = "archivos/neg/Negativo ";
        label = "0";
    }
    ofstream archivo_labeled;
    archivo_labeled.open("archivos/labeledTrainData.tsv", ios::app);

    if(archivo_labeled.fail())
    {
        cout << "Error al abrir el labeledTrainData.tsv" << endl;
    }

    for (int i = 1; i <= 1000; i++)
    {
        ifstream archivo;
        string numeroArchivo = std::to_string(i);
        string nombreArch = nombreArchivo + "(" + numeroArchivo + ").txt";
        archivo.open(nombreArch);
        if(archivo.fail())
        {
            cout << "Error al abrir el archivo " << endl;
        }
        string review = "";
        string reviewCompleto = "";
        while(true)
        {
            getline(archivo, review);
            cout << i << ": " << review << endl;
            if (archivo.eof())
            {
                break;
            }
            reviewCompleto += " " + review;
        }
        archivo_labeled << "\"NIGS_" + numeroArchivo + "_" + label + "\"" << '\t' << label << '\t' << reviewCompleto << '\n';
        nombreArch = "";
    }
    archivo_labeled.close();
}
