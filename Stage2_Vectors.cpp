/*
 * Miguel A. Medina & Diana K. Melo
 * A01023656 - A01023785
 * Programa: solución fase 2/6 RETO
 * Fecha: 15 de octubre de 2020
 * Fecha de corrección: 21 de octubre de 2020
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

class Record
{
public:
    string fecha;
    string hora;
    string ipFuente;
    string puertoFuente;
    string nombreFuente;
    string ipDestino;
    string puertoDestino;
    string nombreDestino;

    Record(string fecha, string hora, string ipFuente, string puertoFuente, string nombreFuente, string ipDestino, string puertoDestino, string nombreDestino)
    {
        this->fecha = fecha;
        this->hora = hora;
        this->ipFuente = ipFuente;
        this->puertoFuente = puertoFuente;
        this->nombreFuente = nombreFuente;
        this->ipDestino = ipDestino;
        this->puertoDestino = puertoDestino;
        this->nombreDestino = nombreDestino;
    }

    void imprimirNombreFuente()
    {
        cout << nombreFuente << endl;
    }
};

vector<Record> conexiones; // conexiones es un vector de Records

void leerDatos(string path)
{
    ifstream fileIn;
    fileIn.open(path);
    string line, partes;
    vector<string> valores;
    while (getline(fileIn, line))
    {
        istringstream sIn(line);
        while (getline(sIn, partes, ','))
        {
            valores.push_back(partes);
        }
        if (valores[7].find('\r') != valores[7].npos)
        {
            valores[7] = valores[7].substr(0, valores[7].size()-1);
        }
        Record r(valores[0], valores[1], valores[2], valores[3], valores[4], valores[5], valores[6], valores[7]);
        conexiones.push_back(r);
        valores.clear();
    }
}

class ConexionesComputadora
{
    public:
        // Atributos
        string IP;
        string nombre;
        list<string> conexionesEntrantes;
        list<string> conexionesSalientes;

    public:
        // Constructores
        ConexionesComputadora() = default;
        ConexionesComputadora(string IP, string nombre)
        {
            this->IP = IP;
            this->nombre = nombre;
            //this->conexionesEntrantes = conexionesEntrantes;
            //this->conexionesSalientes = conexionesSalientes;
        }
        // Métodos
        void insertConexiones(vector<Record> vectorR, string nuestraIP)
        {
            for (int i = 0; i < vectorR.size(); i++)
            {
                if (vectorR[i].ipDestino == nuestraIP)
                {
                    conexionesEntrantes.push_front(vectorR[i].ipDestino);
                }
                else if (vectorR[i].ipFuente == nuestraIP)
                {
                    conexionesSalientes.push_back(vectorR[i].ipDestino);
                }
            }
        }

        void printAllConections()
        {
            for (list<string>::iterator a = conexionesEntrantes.begin(); a != conexionesEntrantes.end(); ++a)
            {
                cout << "Las conexones entrantes son: " << endl;
                cout << " " << *a << endl;
            }

            for (list<string>::iterator b = conexionesSalientes.begin(); b != conexionesSalientes.end(); ++b)
            {
                cout << "Las conexones salientes son: " << endl;
                cout << " " << *b << endl;
            }
        }

        int contarTripletes(vector<Record> a)
        {
            // Pregunta extra:
            int i = 0;
            int counter = 0;
            while (i < a.size())
            {
                if (a[i].ipFuente == a[i+1].ipFuente && a[i].nombreDestino == a[i+1].nombreDestino)
                {
                    if (a[i].ipFuente == a[i+2].ipFuente && a[i].nombreDestino == a[i+2].nombreDestino && a[i].nombreDestino != "-")
                    {
                        counter ++;
                        i++;
                    }
                    else
                    {
                        i++;
                    }
                }
                else
                {
                    i++;
                }
            }
            return counter;
        }
};

int main()
{
    leerDatos("/Users/FuhrerMaicol/Documents/nuevo5.csv");
    cout << "Datos leidos: " << conexiones.size() << endl;
    string IPPublica = "10.109.228.";
    string nuestraIP;
    cout << "Introduce un número entre 1 y 150: " << endl;
    cin >> nuestraIP;
    IPPublica.append(nuestraIP);
    cout<< "La IP es: " << IPPublica << endl;

    int i = 0;
    while (conexiones[i].ipFuente != IPPublica)
    {
        i++;
    }
    string nombre = conexiones[i].nombreFuente;

    auto cc = ConexionesComputadora(IPPublica, nombre);
    cc.insertConexiones(conexiones, IPPublica);
    //cc.printAllConections();
    cout << "El nombre de la computadora ligada a dicha IP es: " << cc.nombre << endl;
    cout << "Hay " << cc.conexionesEntrantes.size() << " conexiones entrantes." << endl;
    cout << "Hay " << cc.conexionesSalientes.size() << " conexiones salientes." << endl;
    cout << "La última conexión que recibió la computadora fue: " << cc.conexionesEntrantes.front() << endl;
    cout << "Veces que hay 3 datos contiguos dentro de todas las conexiones: " << cc.contarTripletes(conexiones) << endl;
    return 0;
}
