/* 
 * File:   GestionVacunas.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 21 de octubre de 2021, 13:03
 */

#include "GestionVacunas.h"

/**
 * @brief Constructor por defecto de la clase GestionVacunas
 * @param nomFichUsuarios: nombre del fichero de Usuarios del que se creará un AVL
 * @param nomFichDosis: nombre del fichero de Dosis del que se creará un VDinámico
 */
GestionVacunas::GestionVacunas( std::string nomFichUsuarios, std::string nomFichDosis ){
    //Instanciamos el AVL de usuarios usando nomFichUsuarios
    ifstream iss(nomFichUsuarios);
    string palabra;
    int corte = 0;

    std::string nombre;
    std::string apellidos;
    std::string NSS;
    int dia = 0;
    int mes = 0;
    int anno = 0;
//  int lat;
//  int lon;

    while (getline(iss, palabra)) {

            corte = palabra.find(';');
            nombre = (palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find(';');
            apellidos = (palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find(';');
            NSS = (palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find('/');
            dia = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find('/');
            mes = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            anno = stoi(palabra);

            Fecha fNac = Fecha(dia,mes,anno);
            Usuario u = Usuario(nombre, apellidos, NSS, fNac);
            _usuarios.inserta(u);
    }
    iss.close();
    //Abrimos el fichero y creamos las variables para la lectura de los datos de nomFichDosis
    ifstream is(nomFichDosis);

    int id = 0;
    int idLote = 0;
    int fabricante = 0;

    while (getline(is, palabra)) {

            corte = palabra.find(';');
            id = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find(';');
            idLote = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find(';');
            fabricante = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find('/');
            dia = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find('/');
            mes = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            anno = stoi(palabra);

            //Convertimos int a tipo fabricante ( enum )
            Dosis::fabricante enumFabricante;

            switch(fabricante){
                case 0: enumFabricante = Dosis::Pfizer;
                break;

                case 1: enumFabricante = Dosis::Moderna;
                break;

                case 2: enumFabricante = Dosis::AstraZeneca;
                break;

                case 3: enumFabricante = Dosis::Johnson;
                break;
            }
            //Creamos una dosis y la insertamos en el vector dinámico de dosis
            Fecha ff = Fecha(dia,mes,anno);
            Fecha fc;
            fc = ff;
            fc.anadirMeses(2); 
            Dosis d = Dosis(id,idLote,enumFabricante,Dosis::enAlmacen,ff,fc);
            _almacena.insertar(d);    
    }
    is.close();
}

/**
 * @brief Constructor copia de la clase GestionVacunas
 * @param orig: objeto tipo GestionVacunas a copiar
 */
GestionVacunas::GestionVacunas(const GestionVacunas& orig):
_usuarios(orig._usuarios),_almacena(orig._almacena){
}

GestionVacunas::~GestionVacunas() {
}

Usuario* GestionVacunas::buscarUsuario( std::string NSS ){
    Usuario result, buscar = Usuario("","",NSS,Fecha() );
    Usuario *p = _usuarios.buscaNR(buscar,result);
    if( p != 0 ){
        return p;
    }
    return 0;
}

/**
 * @brief Función que administra una Dosis a un Usuario según el fabricante recomendado, si no queda inventario se le asigna cualquiera
 * @param u: Usuario
 * @param fabricante: fabricante de la Dosis
 * @return true si se le administra la que corresponde, false en caso contrario
 */
bool GestionVacunas::administrarDosis( Usuario *u, Dosis::fabricante fabricante ){
    
    //Si es menor de 12 años no se vacuna
    if( fabricante == Dosis::null )
        return false;
    
    //Se busca la dosis recomendable
    for( int i = 0; i < _almacena.tamLog(); i++ ){
        
        if( fabricante == _almacena[i].getFabricante() && _almacena[i].getEstado() == Dosis::enAlmacen ){ //==Fabricante y en almacén
            Dosis *d = &_almacena[i];
            u->administrarDosis( d );
            _almacena[i].setEstado( Dosis::administrada );
            return true;
        }  
    }
    //Si no quedan dosis recomendables, se le administra otra distinta que haya en el almacén
    for( int i = 0; i < _almacena.tamLog(); i++ ){
        
        if( _almacena[i].getEstado() == Dosis::enAlmacen ){
            u->administrarDosis( &_almacena[i] );
            _almacena[i].setEstado( Dosis::administrada );
            return false;
        }
    }
    return false;
}

/**
 * @brief Función que calcula el porcentaje total de personas que recibieron la pauta completa de vacunación. Se considera completa si:
 *      - Edad <75 y 2+ dosis
 *      - Edad >=75 y 3+ dosis
 * @return Porcentaje de personas con la pauta completa
 */
int GestionVacunas::pautaCompleta(){
    VDinamico<Usuario*> vUsuarios;
    vUsuarios = _usuarios.recorreInorden();
    
    int cont = 0;
    for( int i = 0; i < vUsuarios.tamLog(); i++ ){
        //std::cout << vUsuarios[i]->getNDosis() << vUsuarios[i]->getDosis().tamLog() << std::endl;
        if( vUsuarios[i]->edad() < 75 && vUsuarios[i]->getNDosis() == 2 )
            cont++;
        else if( vUsuarios[i]->edad() >= 75 && vUsuarios[i]->getNDosis() == 3 )
            cont++;
    }
    return ( cont * 100/vUsuarios.tamLog() );
}

/**
 * @brief Función que devuelve un VDinamico<Usuario*> con aquellos Usuarios que recibieron una vacuna no recomendable
 * @return VDinamico<Usuario*> de los Usuarios que cumplen la condición
 */
VDinamico<Usuario*> GestionVacunas::listadoVacunacionNR(){
    //Copiamos AVL Usuario* a vUsuarios
    VDinamico<Usuario*> vUsuarios;
    vUsuarios = _usuarios.recorreInorden();
    //Vector que se devuelve
    VDinamico<Usuario*> vNR;
    //Datos de cada Usuario*
    VDinamico<Dosis*> vDosis;
    Dosis::fabricante dosisRecom;
    bool comprobar;
    
    for( int i = 0; i < vUsuarios.tamLog(); i++ ){
        
        dosisRecom = vUsuarios[i]->getDosisRecomendable();
        vDosis = vUsuarios[i]->getDosis();
        //Comparamos la dosis recomendable con cada una de las dosis, si se incumple se añade a vNR
        for( int j = 0; j < vDosis.tamLog(); j++ )
            if( dosisRecom != vDosis[j]->getFabricante() )
                comprobar = true;

        if( comprobar ){
            vNR.insertar( vUsuarios[i] );  
        }
        comprobar = false;
        
    }
    return vNR;
}

/**
 * @brief Función que devuelve la cantidad de vacunas que aún se encuentran en el almacén sin inyectarse en un Usuario
 * @return cont: número de vacunas que no se administraron
 */
int GestionVacunas::vacunasAlmacen(){
    
    int cont = 0;
    
    for( int i = 0; i < _almacena.tamLog(); i++ )
        if( _almacena[i].getEstado() == Dosis::enAlmacen )
            cont++;
    
    return cont;
}

/**
 * @brief Función para comprobar si se cargaron el AVL<Usuario> y el VDinamico<Dosis>
 */
void GestionVacunas::estadoCarga(){
    std::cout << "Árbol de Usuarios cargado con --> " << _usuarios.numElementos() << " usuarios." << std::endl;
    std::cout << "Altura del AVL de Usuarios ---> " << _usuarios.altura() << std::endl;
    std::cout << "VDinámico de Dosis cargado con -> " << _almacena.tamLog() << " dosis." << std::endl;
}

void GestionVacunas::mostrarTodasDosis(){
    
    for( int i = 0; i < _almacena.tamLog(); i++){
        std::cout << i+1 << "\t --> ";
        _almacena[i].mostrarDosis();
    }
}

/**
 * @brief Función para conseguir un listado ordenado de todos los NSS de los usuarios del AVL
 * @return VDinamico<std::string> con los NSS
 */
VDinamico<std::string> GestionVacunas::listadoNSS(){
    VDinamico<Usuario*> vAux;
    VDinamico<std::string> vNSS;
    //Obtenemos el vector de Usuario* y pasamos los NSS a un vector auxiliar
    vAux = _usuarios.recorreInorden();
    for( int i = 0; i < vAux.tamLog(); i++ ){
        vNSS.insertar( vAux[i]->GetNSS() );
    }
    return vNSS;
}