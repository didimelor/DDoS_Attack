/* Miguel Medina - A01023656
 * Diana Melo - A01023785
 * Entrega Reto 5/6
 * 26 de noviembre de 2020
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

template <class T>
class Nodo
{
    public:
        T value;
        unordered_set <Nodo<T>*> siguientes;

        Nodo(T value)
        {
            this->value = value;
        }

        void agregarArcoDirigido(Nodo<T> *next)
        {
            if (siguientes.find(next) == siguientes.end())
            {
                siguientes.insert(next);
            }
        }

        void imprimirNodo()
        {
            cout << value << ":" << endl;
            for (auto &itr : siguientes)
            {
                cout << itr << ", ";
            }
            cout << endl;
        }
        
        int tamanioConex ()
        {
            //Deberia ser -1?
            return siguientes.size();
        }
};

template <class T>
class Graph
{
    public:
        unordered_map <string, Nodo<T>*> nodos;

        void agregarNodo(string value)
        {
            if (nodos.find(value) == nodos.end())
            {
                auto *nuevo = new Nodo<T>(value);
                nodos[value] = nuevo;
            }
        }

        void agregarArcoDirigido(string value1, string value2)
        {
            if (nodos.find(value1) != nodos.end() && nodos.find(value2) != nodos.end())
            {
                nodos[value1]->agregarArcoDirigido(nodos[value2]);
            }
        }

        void imprimirNodos()
        {
            for (auto &nodoX : nodos)
            {
                cout << nodoX.first << endl;
            }
        }

        void imprimirGrafo()
        {
            for (auto &itr : nodos)
            {
                cout << itr.second->imprimriNodo();
            }
        }

        //Necesitamos las conexiones salientes de A o las computadoras que se conectaron a A (entrantes)
        int conexionesNodo(string value)
        {
            auto itr = nodos.find(value);
            if(itr == nodos.end())
            {
                cout << "No se encontro la ip: " << endl;
                return 0;
            }
            cout << "Numero de computadoras a las que se conecto  A:  ";
            return itr->second->tamanioConex(); //-1?
        }

};

bool check(string nombreFuenteoDestino, string reto)
{
    size_t encontrado = nombreFuenteoDestino.find(reto);
    if (encontrado != string::npos)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool conectados(string value, unordered_set<string>mySet)
{
    auto itr = mySet.find(value);
    if(itr == mySet.end())
    {
        mySet.insert(value);
        return false;
    }
    else
    {
        return true;
    }
}

void nuevaFuncion(vector<Record> r, string fechaActual, Graph<string> g, string ipBuscar, Graph<string>g2, string B, string C)
{
    unordered_set<string> yaConectadosA;
    unordered_set<string> yaConectadosB;
    unordered_set<string> yaConectadosC;
    string extensionReto = "10.109.228";
    int countConexionA = 0;
    int countConexionB = 0;
    int countConexionC = 0;
    cout << "\nFecha: " << fechaActual << endl;
    
    for (int i = 0; i < r.size(); i++)
    {
        if (r[i].fecha == fechaActual)
        {
            if (r[i].ipFuente != "-" && r[i].ipDestino != "-")
            {
                //aqui se insertan los datos del segundo grafo
                g2.agregarNodo(r[i].ipFuente);
                g2.agregarNodo(r[i].ipDestino);
                //Podemos hacer lo del set Aqui tambien
                g2.agregarArcoDirigido(r[i].ipFuente, r[i].ipDestino);
                if(check(r[i].ipFuente, extensionReto) && r[i].nombreDestino == B)
                {
                    //Si no esta dentro del set se suma al contador
                    auto itrB = yaConectadosB.find(r[i].ipFuente);
                    if(itrB == yaConectadosB.end())
                    {
                        yaConectadosB.insert(r[i].ipFuente);
                        countConexionB++;
                    }
                }
                if(check(r[i].ipFuente, extensionReto) && r[i].nombreDestino == C)
                {
                    auto itrC = yaConectadosC.find(r[i].ipFuente);
                    if(itrC == yaConectadosC.end())
                    {
                        yaConectadosC.insert(r[i].ipFuente);
                        countConexionC++;
                    }
                }
               if (check(r[i].ipFuente, extensionReto) && check (r[i].ipDestino, extensionReto))
                {
                    g.agregarNodo(r[i].ipFuente);
                    g.agregarNodo(r[i].ipDestino);
                    g.agregarArcoDirigido(r[i].ipFuente, r[i].ipDestino);
                    if(r[i].ipDestino == ipBuscar)
                    {
                        auto itrA = yaConectadosA.find(r[i].ipFuente);
                        if(itrA == yaConectadosB.end())
                        {
                            yaConectadosA.insert(r[i].ipFuente);
                            countConexionA++;
                        }

                    }
                }
            }
        }
    }
    cout << g.conexionesNodo(ipBuscar) << endl;
    cout << "Hay: " << countConexionA <<" computadora(s) que se conectaron a A" << endl;
    cout << "Hay: " << countConexionB <<" computadora(s) de reto.com que se conectaron a B" << endl;
     cout << "Hay: " << countConexionC <<" computadora(s) de reto.com que se conectaron a C" << endl;
    countConexionA = 0;
    countConexionB = 0;
    countConexionC = 0;
}

vector<string> fechas(vector<Record>r)
{
    vector<string> fechasUnicas;
    for (int i = 0; i < r.size(); i++)
    {
        auto itr = find(fechasUnicas.begin(),fechasUnicas.end(),r[i].fecha);
        if(itr == fechasUnicas.end())
        {
            fechasUnicas.push_back(r[i].fecha);
        }
    }
    return fechasUnicas;
}

int main()
{
    leerDatos("/Users/didimelor/Desktop/nuevo5 (1).csv");
    cout << "Datos leidos: " << conexiones.size() << endl;

    string A = "10.109.228.113";  // su nombre mary.reto.com
    string B = "ga989wpiem9csgi66e7f.org";
    string C = "yellowpages.com";
    Graph<string> misConexionesCompusInternas;
    Graph<string> misConexionesSitioWeb;
    vector<string> fechasUnicas = fechas(conexiones);
    for(int i=0; i<fechasUnicas.size();i++)
    {
        nuevaFuncion(conexiones, fechasUnicas[i], misConexionesCompusInternas, A, misConexionesSitioWeb,B,C);
    }

    return 0;
}
