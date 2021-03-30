/*
 * Miguel A. Medina R. A01023656
 * Diana M. A01023785
 * 28/09/20
 *
 * Primera evidencia del reto
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Record
{
    public:
        string fecha;
        string hora;
        string nombreFuente;
        string nombreDestino;
        string direccion;
        string puertoDestino;

        Record(string fecha, string hora, string nombreFuente, string direccion, string puertoDestino, string nombreDestino)
        {
            this->fecha = fecha;
            this->hora = hora;
            this->nombreFuente = nombreFuente;
            this->nombreDestino = nombreDestino;
            this->direccion = direccion;
            this->puertoDestino = puertoDestino;
        }

        void imprimirNombreFuente()
        {
            cout << nombreFuente << endl;
        }
        void imprimirFecha()
        {
            cout << fecha << endl;
        }
        void imprimirHora()
        {
            cout << hora << endl;
        }
        void imprimirNombreDestino()
        {
            cout << nombreDestino << endl;
        }
};

vector<Record> conexiones; // conexiones es un vector de Records,

void leerDatos(string path)
{
    ifstream fileIn; // fileIn(objeto de tipo ifstream) = archivo de entrada, requerimos de la librería fstream. ifstream = input file stream = flujo de archivo de entrada.
    fileIn.open(path); // se abre el archivo, en este caso a path, para poder leerlo.
    string line, partes; // declaración de 2 strings.
    vector<string> valores;
    while (getline(fileIn, line)) // lo que lea de fileIn lo meto en line.
    {
        istringstream sIn(line); // sIn de tipo istringstream = string de entrada recibe un line y separa line por las comas.
        while (getline(sIn, partes, ',')) // lee cada línea y cada que ve una coma separa el string y lo guarda en partes.
        {
            valores.push_back(partes); // guardo toda la línea en este vector, elemento por elemento, ya separado.
        }
        if (valores[7].find('\r') != valores[7].npos) // '\r' es retorno de carro que usaban las máquinas de escribir. npos es cómo el No encontrado, es la posición afuera del string.
        {
            valores[7] = valores[7].substr(0, valores[7].size()-1); // si esta lo quitamos al hacer un substring sin ese caractér al restarle uno a size, porque \r está al final del string de manera invisible.
        }
        Record r(valores[0], valores[1], valores[4], valores[5], valores[6], valores[7]); // Construcción de un Record llamado r.
        conexiones.push_back(r); // introduzco el Record r en conexiones. Conexiones es un vector que contiene el vector Record que contiene el vector valores que contiene los datos.
        valores.clear(); // borra el contenedor, en este caso el vector valores.
    }
    // es importante quitar el \r porque sino al comprar palabras tendríamos problemas como que "palabra" y "palabra\r" nunca van a ser iguales, aunque deberían.
}

int compararPorNombreFuente(Record a, Record b) // recibe dos Records para comparar.
{
    if (a.nombreFuente < b.nombreFuente) // las comparaciones se hacen de forma lexicográficas = alfabeticamente y por el tamaño de la palabra en longuitud Ana va antes que Anastasia. Caso de C++.
    {
        return -1;
    }
    else if (a.nombreFuente == b.nombreFuente)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int compararPorNombreDestino(Record a, Record b)
{
    if (a.nombreDestino < b.nombreDestino)
    {
        return -1;
    }
    else if (a.nombreDestino == b.nombreDestino)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int compararPorDia(Record a, Record b)
{
    if (a.fecha < b.fecha)
    {
        return -1;
    }
    else if (a.fecha == b.fecha)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int compararPorPuertoDestino(Record a, Record b)
{
    if (a.puertoDestino < b.puertoDestino)
    {
        return -1;
    }
    else if (a.puertoDestino == b.puertoDestino)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

template <class T>
class Sort // la incluyo para ordenar mis datos.
{
    public:
        virtual void sort(vector<T> &data, int comparador(T a, T b)) = 0; // función virtual, se le agrega comparador.
        void intercambiar(vector<T> &data, int posI, int posJ)
        {
            T temp = data[posI];
            data[posI] = data[posJ];
            data[posJ] = temp;
        }

        void imprimir(vector<T> data)
        {
            for (int i = 0; i < data.size(); i++)
            {
                cout << data[i] << ",";
            }
            cout << endl;
        }
};

template <class T>
class MergeSort : public Sort<T>  // usa bastante RAM, pero es el más rápido.
{

    public:
        void sort(vector<T> &data, int comparador(T a, T b))
        {
            sortAux(data, 0, data.size()-1, comparador);
        }
    private:
        void sortAux(vector<T> &data, int lo, int hi, int comparador(T a, T b))
        {
            if  (lo >= hi)
            {
                return;
            }

            int mid = (lo+hi)/2;
            sortAux(data, lo, mid, comparador);
            sortAux(data, mid+1, hi, comparador);
            merge(data, lo, mid, hi, comparador);
        }

        void merge(vector<T> &data, int low, int mid, int hi, int comparador(T a, T b)) // nueva función que recibe 3 enteros, nuestro vector y lo que regresa la función comparador que recibe dos elementos de tipo T [truco de C++].
        {
            int sizeA=mid-low+1;
            int sizeB=hi-mid;
            vector<T> copiaA;
            vector<T> copiaB;

            for (int i = 0; i<sizeA; i++)
            {
                copiaA.push_back(data[low+i]);
            }

            for (int i = 0; i<sizeB; i++)
            {
                copiaB.push_back(data[mid+i+1]);
            }

            int indexA = 0;
            int indexB = 0;
            int indexData = low;

            while (indexA<sizeA && indexB < sizeB)
            {
                //if(copiaA[indexA]<copiaB[indexB]){ // versión antes del comparador.
                if (comparador(copiaA[indexA], copiaB[indexB]) < 0) // checar si lo que está en la copia A es menor que lo que en la copia B, para ello también se usa el cero.
                {
                    data[indexData]  = copiaA[indexA];
                    indexA++;
                }
                else // se usa la copiaB si no se cumple lo de arriba.
                {
                    data[indexData] = copiaB[indexB];
                    indexB++;
                }
                indexData++;
            }

            while (indexA < sizeA)
            {
                data[indexData] = copiaA[indexA];
                indexA++;
                indexData++;
            }

            while (indexB < sizeB)
            {
                data[indexData] = copiaB[indexB];
                indexB++;
                indexData++;
            }
        }
};

int compararPorNombreFuente2(Record a, string b) // TEST exitoso
{
    if (a.nombreFuente < b)
    {
        return -1;
    }
    else if (a.nombreFuente == b)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int compararPorNombreDestino2(Record a, string b)
{
    if (a.nombreDestino < b)
    {
        return -1;
    }
    else if (a.nombreDestino == b)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int compararPorPuertoDestino2(Record a, string b)
{
    if (a.puertoDestino < b)
    {
        return -1;
    }
    else if (a.puertoDestino == b)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void contarDia(vector <Record> v)
{
    string diaUno = v[0].fecha;
    int i = 0;
    while (v[i].fecha == diaUno)
    {
        i++;
    }
    string diaDos = v[i].fecha;
    int contadorDiaDos = 0;
    while (v[i].fecha == diaDos)
    {
        i++;
        contadorDiaDos++;
    }
    cout << "El segundo dia registrado es el dia: "<< diaDos <<" en ese dia se registraron: " << contadorDiaDos << " records" << endl;
    string dia1 = v[0].fecha;
}

template <class T, class M>
int busquedaBinaria2(vector<T> a, M buscado, int comparador(T a, M b))
{
    int inicio = 0;
    int fin = a.size()-1;
    while (fin >= inicio)
    {
        int medio = (inicio + fin)/2;
        if (comparador(a[medio],buscado) == 0)
        {
            return medio;
        }
        else if (comparador(a[medio], buscado) < 0)
        {
            inicio = medio+1;
        }
        else
        {
            fin = medio-1;
        }
    }
    return -1;
}

template <class T>
int busquedaBinaria(vector<T> a, T buscado, int comparador(T a, T b))
{
    int inicio = 0;
    int fin = a.size()-1;
    while (fin >= inicio){
        int medio = (inicio+fin)/2;
        if (comparador(a[medio],buscado) == 0)
        {
            return medio;
        }
        else if (comparador(buscado,a[medio]) > 0)
        {
            inicio = medio + 1;
        }
        else
        {
            fin = medio-1;
        }
    }
    return -1;
}

int main()
{
    leerDatos("/Users/FuhrerMaicol/Downloads/equipo 5.csv");
    cout << "Datos leidos" << endl;
    MergeSort<Record> s;
    s.sort(conexiones, compararPorNombreFuente); // Aquí le digo que el comparador será el nombreFuente.
    cout << "Datos ordenados" << endl;
    cout << "El total de registros son: " << conexiones.size() << endl;
    s.sort(conexiones, compararPorNombreDestino);
    //cout << "Nombres destino ordenados" << endl;
    s.sort(conexiones, compararPorDia);
    cout<<"Datos ordenados de acuerdo al día" << endl;
    contarDia(conexiones);
    s.sort(conexiones, compararPorPuertoDestino);
    cout << "Puertos destino ordenados" << endl;
    //mostrarPuertosDestino(conexiones);

    //Record r("", "", "jeffrey.reto.com", "", "", "");
    Record r("", "", "", "", "", "gmail.com");
    string a = "gmail.com"; // palabra a buscar

    int pos = busquedaBinaria2(conexiones, a, compararPorNombreDestino2);
    if (pos != -1)
    {
        cout << "encontrado" << endl;
    }

    cout << "Los puertos menores a mil son: " << endl;
    for (int puerto = 1; puerto < 1000; puerto++)
    {
        int posPuerto = busquedaBinaria2(conexiones, to_string(puerto), compararPorPuertoDestino2);
        if(posPuerto != -1)
        {
            cout << puerto << endl;
        }
    }
    return 0;
}
