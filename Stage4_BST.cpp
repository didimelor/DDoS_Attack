/*
 * Miguel A. Medina R. - A01023656
 * Diana Melo - A01023785
 * Entrega Reto 4/6
 * 19 de noviembre de 2020
 * Corrección: 24 de noviembre de 2020
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>

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

vector<Record> conexiones;

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

bool check(string nombreFuenteoDestino, string reto)
{
    // condition to check if A (nombreFuenteoDestino) is bigger than or equal to B (reto), otherwise B can't be a substring of A.
    if (nombreFuenteoDestino.length() >= reto.length())
    {
        // if the condition is true, then a substring of "a" is created.
        string subString = nombreFuenteoDestino.substr(nombreFuenteoDestino.size() - reto.size(), nombreFuenteoDestino.size());

        // string "reto" and "subString" are compared to determine if they are alike or not.
        return reto == subString;
    }
    else
    {
        return false;
    }
}

template <class T>
class Nodo
{
    public:
        T value;
        string sitio;
        Nodo<T> *izq;
        Nodo<T> *der;

        Nodo()
        {
            value = NULL;
            sitio = NULL;
            izq = NULL;
            der = NULL;
        }
        Nodo(T value, string sitio)
        {
            this->value = value;
            this->sitio = sitio;
            izq = NULL;
            der = NULL;
        }
};

template <class T>
class Arbol
{
    public:
        int n;
        Nodo<T> *root;

        Arbol()
        {
            root = NULL;
        }

        // MAIN
        void insertar(T value, string sitio)
        {
            auto *nuevo = new Nodo<T>(value, sitio);
            if  (root == NULL)
            {
                root = nuevo;
            }
            else
            {
                insertar(nuevo, root);
            }
        }
        // HELPER
        void insertar(Nodo<T> *nuevo, Nodo<T> *temp)
        {
            //si el valor es igual, terminamos
            if (nuevo->value == temp->value)
            {
                return;
            }
            else if (nuevo->value > temp->value)
            {
                if (temp->der == NULL)
                {
                    temp->der = nuevo;
                }
                else
                {
                    insertar(nuevo, temp->der);
                }
            }
            else if (nuevo->value < temp->value)
            {
                if (temp->izq == NULL)
                {
                    temp->izq = nuevo;
                }
                else
                {
                    insertar(nuevo, temp->izq);
                }
            }
        }

        // MAIN
        void inorder(int n)
        {
            this->n = n;
            inorder(root);
            cout << endl;
        }
        // HELPER
        void inorder(Nodo<T> *temp)
        {
            if(temp != NULL)
            {

                inorder(temp->der);
                if (this->n > 0)
                {
                    cout << temp->value << ", " << temp->sitio << endl;
                }
                this->n = this->n - 1;
                inorder(temp->izq);
            }
        }
};

unordered_map <string, int> ordenarPorDia(vector<Record> r, string fechaActual)
{
    unordered_map <string, int> myMap;
    string extensionReto = ".reto.com";
    string ipDestinoActual;
    string nombreDestinoActual;

    for (int i = 0; i < r.size(); i++)
    {
        if (r[i].fecha == fechaActual)
        {
            // check that ND doesn't have the extension ".reto.com"
            if (!check(r[i].nombreDestino, extensionReto))
            {
                ipDestinoActual = r[i].ipDestino;
                nombreDestinoActual = r[i].nombreDestino;
                auto noDouble = myMap.find(nombreDestinoActual);

                if (r[i].ipDestino != "-" && r[i].nombreDestino != "-")
                {
                    // it doesn't exist, it's inserted.
                    if (noDouble == myMap.end())
                    {
                        // llave = nombreDestino, valor = num conexiones entrantes.
                        pair<string, int> pairToInsert (nombreDestinoActual, 1);
                        myMap.insert(pairToInsert);
                    }
                    // it already exists.
                    else
                    {
                        myMap[nombreDestinoActual]++;
                    }
                }
            }
        }
    }
    return myMap;
}

void top(int n, string fecha)
{
    unordered_map <string, int> a = ordenarPorDia(conexiones, fecha);
    // creación del BST
    Arbol <int> arbol;
    for (auto &x : a)
    {
        arbol.insertar(x.second, x.first);
    }
    arbol.inorder(n);
}

// función para insertar fechas únicas en un set.
vector<string> fechas(vector<Record> r)
{
    vector<string> fechasUnicas;
    string fechaActual;

    for (int i = 0; i < r.size(); i++)
    {
        fechaActual = r[i].fecha;
        auto it = find(fechasUnicas.begin(), fechasUnicas.end(), fechaActual);
        if (it == fechasUnicas.end())
        {
            fechasUnicas.push_back(fechaActual);
        }
    }
    return fechasUnicas;
}

int main()
{
    leerDatos("/Users/FuhrerMaicol/Documents/nuevo5.csv");
    cout << "Datos leidos: " << conexiones.size() << endl;

    //================== RETO 4 ==================//
    unordered_set<string> nombresTop;
    std::string fechaBuscada;
    std::cout << "Introduce la fecha que quieras buscar: ";
    std::cin >> fechaBuscada;

    vector<string> FU = fechas(conexiones);

    for (auto &l : FU)
    {
        cout << "Fecha: " << l << endl;
        top(5, l);
    }

    int counter = 0;
    cout << nombresTop.size() << endl;
    for (auto &w : nombresTop)
    {
        auto itr = nombresTop.find(w);
        while (itr != nombresTop.end())
        {
            auto itr2 = nombresTop.find(w);
            counter++;
            itr++;
            itr2++;
        }
        if (counter == FU.size() || counter > FU.size())
        {
            cout << "Este sitio aparece en el top todos los días: " << w << endl;
        }
        counter = 0;
    }
    return 0;
}
