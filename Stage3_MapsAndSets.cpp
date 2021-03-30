#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstddef>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

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

        Record(string fecha, string hora, string ipFuente, string puertoFuente,string nombreFuente, string ipDestino, string puertoDestino, string nombreDestino)
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

vector<Record> conexiones; // conexiones es un vector de Records,

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
        Record r(valores[0], valores[1], valores[2],valores[3],valores[4], valores[5], valores[6], valores[7]);
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

        ConexionesComputadora() = default;
        ConexionesComputadora(string IP, string nombre){
            this->IP = IP;
            this->nombre = nombre;
            
        }
        // Métodos
        void insertConexiones(vector<Record> vectorR, string nuestraIP)
        {
            for (int i = 0; i < vectorR.size(); i++)
            {
                if (vectorR[i].ipDestino == nuestraIP && vectorR[i].ipDestino != "-")
                {
                    conexionesEntrantes.push_front(vectorR[i].ipFuente);
                }
                else if (vectorR[i].ipFuente == nuestraIP && vectorR[i].ipFuente != "-")
                {
                    conexionesSalientes.push_back(vectorR[i].ipDestino);
                }
            }
        }

        void printAllConections()
        {
            for (list<string>::iterator a = conexionesEntrantes.begin(); a != conexionesEntrantes.end(); ++a)
            {
                cout << "Las conexiones entrantes son: " << endl;
                cout << " " << *a << endl;
            }

            for (list<string>::iterator b = conexionesSalientes.begin(); b != conexionesSalientes.end(); ++b)
            {
                cout << "Las conexiones salientes son: " << endl;
                cout << " " << *b << endl;
            }
        }
};


void conexionesEntrantesCompu(unordered_map<string, ConexionesComputadora> TodasConexiones, int contador){
    string buscado;
    while(contador >0){
        cout << "Dame la conexion de la que quieres imprimir las conexiones entrantes "<<endl;
        cin >> buscado;
        auto it = TodasConexiones.find(buscado);
        if(it != TodasConexiones.end()){
            cout << "Computadora con nombre: " << it->second.nombre << "De ip: " <<it->second.IP << "Conexiones entrantes: " << endl;
            
            for(auto itCE = it->second.conexionesEntrantes.begin(); itCE != it->second.conexionesEntrantes.end(); itCE++){
                cout << *itCE << ","<<endl;
                if(*itCE== "ga989wpiem9csgi66e7f.org" or *itCE == "s6z7w3sizgaem4fc4x6t.xxx"){
                    cout <<"La computadora se conecto al dominio anomalo: " << *itCE << endl;
                }
            }
        }
        else{
            cout << "No se encontro la ip" <<endl;
        }
        contador--;
    }
}


bool check (string nombre, string substrNom){
    int initializeSubstr = nombre.size() - substrNom.size();
    if(initializeSubstr<0){
        return false;
    }
    else if (substrNom != nombre.substr(initializeSubstr,nombre.size())){
        return true;
    }
    else{
        return false;
    }
}

bool checkIPS (string ip, string substrIP){
    if(ip.substr(0,11) == substrIP){
        return true;
    }
    return false;
}

int main(int argc, const char * argv[]) {
    
    leerDatos("/Users/didimelor/Downloads/nuevo5 (1).csv");
       cout << "Datos leidos: " << conexiones.size() << endl;
    
    string IPPublica = "10.109.228.";
    string nuestraIP;
    cout << "Introduce un número entre 1 y 150: " << endl;
    cin >> nuestraIP;
    IPPublica.append(nuestraIP);
    cout<< "La IP es: " << IPPublica << endl;

    int i = 0;
    //Aqui se puede hacer un find
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
   
    unordered_set<string> NoReto;
    unordered_set<string> SiReto;
    unordered_map<string, ConexionesComputadora> TodasConexiones;
    
    string nombreDestinoActual;
    string nombreFuenteActual;
    string ipDestinoActual;
    string ipFuenteActual;
    string substrNom = ".reto.com";
    bool esNoReto;
    string ipActual;
    string increment;
    
    for(int j = 0; j < conexiones.size(); j++){
       /* while(conexiones[j].nombreDestino== "-" or conexiones[j].nombreDestino== "-"){
            //cout << "Se detecto un guion" <<endl;
            j++;
        }*/
        nombreDestinoActual = conexiones[j].nombreDestino;
        ipDestinoActual = conexiones[j].ipDestino;
        esNoReto = check(nombreDestinoActual, substrNom);
        if(esNoReto == true){
            NoReto.insert(nombreDestinoActual);
        }
        if(checkIPS(ipDestinoActual, "10.109.228.") && SiReto.find(ipDestinoActual)==SiReto.end()&& ipDestinoActual != "-"){
            //cout << "Se inserto en siReto: "<< ipDestinoActual << endl;
            SiReto.insert(ipDestinoActual);
        }
        if(auto itr= TodasConexiones.find(conexiones[j].ipDestino)== TodasConexiones.end())
        {
            auto cc = ConexionesComputadora (ipDestinoActual,nombreDestinoActual);
            cc.insertConexiones(conexiones, ipDestinoActual);
            TodasConexiones.insert({ipDestinoActual, cc});
            //cout<< "Se inserto el dominio: "<< cc.nombre<< endl;
        }
        nombreFuenteActual = conexiones[j].nombreFuente;
        ipFuenteActual = conexiones[j].ipFuente;
        esNoReto = check(nombreFuenteActual, substrNom);
        if(esNoReto == true)
        {
            NoReto.insert(nombreFuenteActual);
        }
        if(checkIPS(ipFuenteActual, "10.109.228.")&& SiReto.find(ipFuenteActual)== SiReto.end() && ipFuenteActual != "-"){
            SiReto.insert(ipFuenteActual);
        }
        //Aqui deberiamos quitar la el find de la ip?
        if (auto itr2 = TodasConexiones.find(conexiones[j].ipFuente)==TodasConexiones.end())
        {
            auto cc = ConexionesComputadora (ipFuenteActual,nombreFuenteActual);
            cc.insertConexiones(conexiones, ipFuenteActual);
            TodasConexiones.insert({ipFuenteActual, cc});
            //cout<< "Se inserto el dominio: "<< cc.nombre<< endl;
        }
        
        
    }
    
    for (auto itr = NoReto.begin(); itr != NoReto.end(); itr++){
        cout<< *itr << endl;
    }
    
    
    //----------Pregunta 2----------
    string dominioBuscado;
    cout << "Inserte el dominio del que quiera saber la ip: "<< endl;
    cin>> dominioBuscado;
    
    for(auto i = TodasConexiones.begin(); i!= TodasConexiones.end(); i++){
        if(i->second.nombre == dominioBuscado){
           cout << "La ip del dominio "<< dominioBuscado << " es: "<< i->first << endl;
            break;
        }
    }
    
    //--------Pregunta 2-------- (especifica a los anomalos)
    string nombreAnomalo1 = "ga989wpiem9csgi66e7f.org";
    string nombreAnomalo2 = "s6z7w3sizgaem4fc4x6t.xxx";
    for(auto anomalo = TodasConexiones.begin(); anomalo != TodasConexiones.end(); anomalo++){
        if(nombreAnomalo1 == anomalo->second.nombre){
            cout << "La ip del " << nombreAnomalo1 << " es "<< anomalo->second.IP << endl;
        }
        if(nombreAnomalo2 == anomalo->second.nombre){
            cout << "La ip del "<< nombreAnomalo2 << " es "<< anomalo->second.IP << endl;
        }
    }
    
    //---------Pregunta 3------------
    int cuentaAlMenos2CE = 0;
    for(auto i = SiReto.begin(); i!=SiReto.end(); i++){
        auto got = TodasConexiones.find(*i);
        if(got != TodasConexiones.end()){
            if(got->second.conexionesEntrantes.size() >= 1){
                cuentaAlMenos2CE++;
                }
            }
        }

    cout << "Hay " << cuentaAlMenos2CE << " computadoras pertenecientes al .reto.com con al menos dos conexiones entrantes"<< endl;
    //------Pregunta 4---------
    int cuantasConexiones;
    cout << "¿De cuantas computadoras quieres checar las conexiones entrantes?" << endl;
    cin>> cuantasConexiones;
    
    conexionesEntrantesCompu(TodasConexiones, cuantasConexiones);

    
    //--------Pregunta 6--------

    string ipAnomalo1="81.111.164.206";
    string ipAnomalo2= "162.103.26.65";
    cout << "Buscando conexiones salientes de la computadora mary.reto.com para verificar conexiones con dominios anomalos..."<<endl;
    auto encuentraAnomalo = TodasConexiones.find("10.109.228.113");
    for(auto it = encuentraAnomalo->second.conexionesSalientes.begin(); it!=encuentraAnomalo->second.conexionesSalientes.end(); it++){
        if(*it == ipAnomalo1){
            cout << "Se conecto al anomalo de ip: "<<ipAnomalo1 << " de nombre: " << "s6z7w3sizgaem4fc4x6t.xxx" <<endl;
        }
        if(*it == ipAnomalo2){
            cout << "Se conecto al anomalo de ip: "<<ipAnomalo2 << " de nombre: " << "ga989wpiem9csgi66e7f.org"<<endl;
        }
    }
    
    
    
    
    
    return 0;
        
    }
