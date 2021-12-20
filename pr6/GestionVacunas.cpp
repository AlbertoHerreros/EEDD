/* 
 * File:   GestionVacunas.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 21 de octubre de 2021, 13:03
 */

#include "GestionVacunas.h"
#include <math.h> 
#include <stdio.h>
#include <limits.h>
/**
 * @brief Constructor por defecto de la clase GestionVacunas
 * @param nomFichUsuarios: nombre del fichero de Usuarios del que se creará un AVL
 * @param nomFichDosis: nombre del fichero de Dosis del que se creará un VDinámico
 */
GestionVacunas::GestionVacunas( std::string nomFichUsuarios, std::string nomFichDosis, std::string nomFichCentros ){
    _nomFichDosis = nomFichDosis; //Se usará para rellenar los centros según se pida
    //Instanciamos el AVL de usuarios usando nomFichUsuarios
    ifstream is(nomFichUsuarios);
    string palabra;
    int corte = 0;
    std::string nombre;
    std::string apellidos;
    std::string NSS;
    int dia = 0;
    int mes = 0;
    int anno = 0;
    float lat;
    float lon;
    UTM direccion;
    
    //Parámetros para la MallaRegular
    float yMax = -9999;
    float yMin = UINT_MAX;
    float xMax = -9999;
    float xMin = UINT_MAX;
    
    ifstream isss(nomFichCentros);
    int id = 0;
    
    //Centros de vacunación
    while (getline(isss, palabra)) {

            corte = palabra.find(';');
            id = stoi(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            corte = palabra.find(';');
            lat = stof(palabra.substr(0, corte));
            palabra.erase(0, corte + 1);

            lon = stof(palabra);
            
            
            //Creamos el centro y lo insertamos
            direccion = {lat,lon};
            CentroVacunacion cv(id,direccion);
            _centros.push_back(cv);    
    }
    isss.close();
    
    for( int i = 0; i < 5; i++ )
        _centros[i].setAlarma(this);
    
    _lineaDosis = 0;
    //Abrimos el fichero y creamos las variables para la lectura de los datos de nomFichDosis
    ifstream iss(nomFichDosis);
 
    int idLote = 0;
    int fabricante = 0;
    int cont = 0;
    
    std::vector<Dosis*> vCentro1;
    std::vector<Dosis*> vCentro2;
    std::vector<Dosis*> vCentro3;
    std::vector<Dosis*> vCentro4;
    std::vector<Dosis*> vCentro5;
    
    while (getline(iss, palabra) && cont < 29750 ) {
            
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
            Dosis *d = new Dosis(id,idLote,enumFabricante,Dosis::enAlmacen,ff,fc);
            /*
             * Centro 1: 8000 dosis
             * Centro 2: 8200
             * Centro 3: 8500
             * Centro 4: 5000
             * Centro 5: 50
             */
            if( cont < 8000 )
                vCentro1.push_back(d);
            else if( cont >= 8000 && cont < 16200)
                vCentro2.push_back(d);
            else if( cont >= 16200 && cont < 24700 )
                vCentro3.push_back(d);
            else if( cont >= 24700 && cont < 29700 )
                vCentro4.push_back(d);
            else
                vCentro5.push_back(d);
            
            cont++;
            _lineaDosis++;
    }
    //iss.close();
    
    _centros[0].anadirNDosisAlmacen(vCentro1);
    _centros[1].anadirNDosisAlmacen(vCentro2);
    _centros[2].anadirNDosisAlmacen(vCentro3);
    _centros[3].anadirNDosisAlmacen(vCentro4);
    _centros[4].anadirNDosisAlmacen(vCentro5);
    _lineaDosis++;//Arreglo para apuntar bien el índice
    
    std::vector<TarjetaVacunacion*> tarjetasMalla;
    //Usuarios y Tarjetas de vacunación
    while (getline(is, palabra)) {

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
            
            corte = palabra.find(';');
            anno = stoi(palabra.substr(0,corte));
            palabra.erase(0, corte + 1);
            
            corte = palabra.find(';');
            lat = stof(palabra.substr(0,corte));
            palabra.erase(0, corte + 1);
            
            lon = stof(palabra);
            
            if( lat > yMax )
                yMax = lat;
            else if( lat < yMin )
                yMin = lat;
            if( lon > xMax )
                xMax = lon;
            else if( lon < xMin )
                xMin = lon;
            
            Fecha fNac = Fecha(dia,mes,anno);
            
            direccion = {lat,lon};
            Usuario u = Usuario(nombre, apellidos, NSS, fNac, direccion);
            
            std::pair<std::string, Usuario> par;
            par.first = NSS;
            par.second = u;
            _usuarios.insert(par);
            
            //Tarjeta de vacunación
            std::string clave = NSS + nombre;
            
            std::map<std::string, Usuario>::iterator it;
            it=_usuarios.find(NSS);
            
            int centroElegido = centroCercano(&(it->second))->getId();
            TarjetaVacunacion tarjeta( clave, false, centroElegido,&(*it).second );
            _tarjetas.insertar( 0,tarjeta); 
            
            //Preparar cada tarjeta para la malla
            TarjetaVacunacion* t;
            this->buscarTarjeta(clave,t);
            tarjetasMalla.push_back(t);
            
    }
    is.close();
    MallaRegular<TarjetaVacunacion*> aux = MallaRegular<TarjetaVacunacion*>(floor(yMin),floor(xMin),ceil(yMax),ceil(xMax),24);
    for( int i = 0; i < tarjetasMalla.size(); i++){
        aux.insertar( tarjetasMalla[i]->getDomicilio().lat, tarjetasMalla[i]->getDomicilio().lon, tarjetasMalla[i] );
    }
    _malla = aux;
}

/**
 * @brief Constructor copia de la clase GestionVacunas
 * @param orig: objeto tipo GestionVacunas a copiar
 */
GestionVacunas::GestionVacunas(const GestionVacunas& orig):
_usuarios(orig._usuarios),_centros(orig._centros),_lineaDosis(orig._lineaDosis),_tarjetas(orig._tarjetas),_malla(orig._malla){
}

GestionVacunas::~GestionVacunas() {
}

/**
 * @brief Función que busca un Usuario a través de su NSS en un std::map<std::string,Usuario>
 * @param NSS: clave a buscar
 * @return Usuario encontrado, o null si no está
 */
Usuario* GestionVacunas::buscarUsuario( std::string NSS ){
    
    std::map<std::string, Usuario>::iterator it;
    it = _usuarios.find(NSS);
    
    if( it != _usuarios.end() ){
        return &(*it).second;
    }
    return NULL;
}

/**
 * @brief Función para conseguir un listado ordenado de todos los NSS de los usuarios del AVL
 * @return VDinamico<std::string> con los NSS
 */
std::vector<std::string> GestionVacunas::listadoNSS(){
    std::vector<std::string> vNSS;
    //Escritura de los string(clave) de los Usuarios del mapa en el vector auxiliar (vNSS)
    std::map<std::string,Usuario>::iterator it;
    for( it = _usuarios.begin(); it != _usuarios.end(); it++ )
        vNSS.push_back( (*it).first );
    
    return vNSS;
}

/**
 * @brief Función que da el número de dosis total que existe de un determinado fabricante en los CentroVacunacion
 * @param f: fabricante a buscar
 * @return  total dosis
 */
int GestionVacunas::numTotalVacunasTipo( Dosis::fabricante f ){
    int total = 0, cont = 0;
    
    for( int i = 0; i < _centros.size(); i++ ){
        cont = _centros[i].numVacunasTipo(f);
        std::cout << "\t- Centro " << i+1 << " --> " << cont << " dosis." << std::endl;
        total+=cont;
    }
    return total;
}


/**
 * @brief Función que da el CentroVacunación más cercano a un Usuario, además lo vacuna
 * @param u: Usuario al que buscaremos su centro
 * @return CentroVacunacion más cercano a u
 */
CentroVacunacion* GestionVacunas::centroCercano( Usuario *u ){
    
    double distancia = 0;
    double min = UINT_MAX;
    
    CentroVacunacion* cV;
    
    for( int i = 0; i < _centros.size(); i++ ){
        
        distancia = sqrt( (pow( _centros[i].getUTM().lat - u->getUTM().lat,2) + (pow( _centros[i].getUTM().lon - u->getUTM().lon ,2) ) ) );
        
        if( distancia < min){
            min = distancia;
            cV = &_centros[i];
        }   
    }
    
    if( cV ){
        return cV;
    }
    return NULL;
}

bool GestionVacunas::vacunarUsuario( TarjetaVacunacion *t ){
    int pos = t->getIdCentroCercano();
    if(pos >= 1 && pos <= 5){
       _centros[pos-1].anadirTarjetaLista(t);
       return _centros[pos-1].administrarDosis(t,t->getDosisRecomendable() );
    }
}

/**
 * @brief Función que se encarga de mandar dosis al centro que lo solicite ( mientras el fichero tenga )
 * @param centro: CentroVacunacion que solicita Dosis
 * @param nDosis: número de dosis solicitadas ( generalmente 100 )
 */
void GestionVacunas::suministrarNDosisAlCentro( CentroVacunacion *centro, int nDosis ){
        
    ifstream iss(_nomFichDosis);

    if( _lineaDosis >= 30000 ){
        std::cout << "\n Centro " << centro->getId() << " ha solicitado " << nDosis << ". Lamentablemente no quedan dosis disponibles." << std::endl;
        return;
    }
    //Variables auxiliares para la lectura
    string palabra;
    int corte = 0;
    int dia = 0;
    int mes = 0;
    int anno = 0;
    
    int id = 0;
    int idLote = 0;
    int fabricante = 0;
    int cont = 0;
    
    std::vector<Dosis*> vCentro;
    
    while ( getline(iss, palabra) && cont < 100) {
        
        if( palabra != ""){
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
            Dosis *d = new Dosis(id,idLote,enumFabricante,Dosis::enAlmacen,ff,fc);
            
            vCentro.push_back(d);
            cont++;
            _lineaDosis++;
        }
    }
    std::cout << " \n<><><><><><><><><><><><> ¡ALARMA EN EL CENTRO "<< centro->getId() <<"! <><><><><><><><><><><><>\n Se han solicitado --> " << cont << 
            " dosis.  \n<><><><><><><><><><><><> ¡ALARMA EN EL CENTRO " << centro->getId() << "! <><><><><><><><><><><><>\n" << std::endl;
    iss.close();
    centro->anadirNDosisAlmacen(vCentro);
    }

/**
 * @brief Función para comprobar si se cargaron el AVL<Usuario> y el VDinamico<Dosis>
 */
void GestionVacunas::estadoCarga(){
    std::cout << "Mapa de Usuarios cargado con --> " << _usuarios.size() << " usuarios." << std::endl;
    std::cout << "Vector de Centros de Vacunación cargado con " << _centros.size() << " centros." << std::endl;
    for( int i = 0; i < _centros.size(); i++ )
        std::cout << "\t- Centro " << i+1 << " dispone de --> " << _centros[i].getNDosisAlmacenadas() << " dosis." << std::endl;
    
    std::cout << "Tabla Hash: " << std::endl;
    std::cout << "\t- Tamaño físico de la tabla: " << _tarjetas.tamTabla() << std::endl;
    std::cout << "\t- Número de tarjetas que alberga: " << _tarjetas.numTarjetas() << std::endl;
    std::cout << "\t- Factor de carga: " << _tarjetas.factorCarga() << std::endl;
    std::cout << "\t- Promedio de colisiones: " << _tarjetas.promedioColisiones() << std::endl;
    std::cout << "\t- Máximo de colisiones: " << _tarjetas.maxColisiones() << std::endl;
    std::cout << "\t- Veces que ocurrieron más de 10 colisiones: " << _tarjetas.numMax10() << std::endl;
    
    std::cout << "Malla Regular: " << std::endl;
    std::cout << "\t- Promedio de elementos por casilla: " << _malla.promedioElementosPorCelda() << std::endl;
    std::cout << "\t- Máximo de elementos en una casilla: " << _malla.maxElementosPorCelda() << std::endl;

}


void GestionVacunas::insertarTarjeta( TarjetaVacunacion &t ){
    _tarjetas.insertar(0,t);
}

TarjetaVacunacion* GestionVacunas::buscarTarjeta( std::string clave, TarjetaVacunacion *&t ){
   if(_tarjetas.buscar(0,clave,t))
       return t;
   else 
       return NULL;
    
}
void GestionVacunas::borrarTarjeta( TarjetaVacunacion *t ){
    std::string id = t->getId();
    _tarjetas.borrar( 0,id );
}

/**
 * @brief Función que determina si dada una tarjeta de usuario ( y su respectivo usuario ) cumple la nueva pauta completa
 * para el pasaporte covid
 * @param t: TarjetaVacunacion
 * @return true si lo cumple, false si no lo cumple
 */
bool GestionVacunas::pautaCompleta2( TarjetaVacunacion* t){
    Usuario *u = t->getTarjetaUsuario();
    if( u->edad() > 5 && u->edad() <= 60 && t->getNDosis() == 2 )
        return true;
    if( u->edad() > 60 && t->getNDosis() == 3 )
        return true;
    return false;
}

/**
 * @brief Función que determina si dada una tarjeta de usuario ( y su respectivo usuario ) cumple la nueva pauta completa
 * para el pasaporte covid
 * @param t: TarjetaVacunacion
 * @return true si lo cumple, false si no lo cumple
 */
bool GestionVacunas::pautaCompleta3( TarjetaVacunacion* t){
    Usuario *u = t->getTarjetaUsuario();
    if( u->edad() >= 5 && u->edad() <= 11 && t->getNDosis() == 1 )
        return true;
    if( u->edad() > 11 && u->edad() <= 60 && t->getNDosis() == 2 )
        return true;
    if( u->edad() > 60 && t->getNDosis() == 3 )
        return true;
    return false;
}

/**
 * @brief Función que llama a vacunarse a todos los Usuarios menores [5,11] dentro del radio
 * indicado, el radio se aplica a los CentroVacunacion
 * @param edad: edad-1 desde la que se llamarán a vacunar
 * @param radio: radio de la circunferencia para llamar a vacunarse
 * @return vector de IDs de las TarjetaVacunación encontradas en el radio
 */
std::vector<std::string> GestionVacunas::llamadaVacunacionMenores(float radio){
    std::vector<std::string> vId;
    std::vector<TarjetaVacunacion*> vT;
    for( int i = 0; i < _centros.size(); i++ ){
        vT = _malla.buscarRadio( _centros[i].getLatitud(), _centros[i].getLongitud(),radio);
        for( int j = 0; j < vT.size(); j++ ){
            Usuario *u = vT[j]->getTarjetaUsuario();
            if( u->edad() >= 5 && u->edad() <= 11 && vT[j]->getAviso() == "" ){
                vT[j]->setAviso( to_string(_centros[i].getId()) + ": Acuda al centro más cercano para vacunarse.");
                _centros[i].anadirTarjetaLista( vT[j] );
                vId.push_back( vT[j]->getId() );
            }
        }      
    }
    return vId;
}

/**
 * @brief Función que llama a vacunarse a todos los Usuarios mayores que la edad marcada dentro del radio
 * indicado, el radio se aplica a los CentroVacunacion
 * @param edad: edad-1 desde la que se llamarán a vacunar
 * @param radio: radio de la circunferencia para llamar a vacunarse
 * @return vector de IDs de las TarjetaVacunación encontradas en el radio
 */
std::vector<std::string> GestionVacunas::llamadaVacunacionMayoresDe(int edad, float radio){
    std::vector<std::string> vId;
    std::vector<TarjetaVacunacion*> vT;
    for( int i = 0; i < _centros.size(); i++ ){
        
        vT = _malla.buscarRadio( _centros[i].getLatitud(), _centros[i].getLongitud(),radio);
        
        for( int j = 0; j < vT.size(); j++ ){
            Usuario *u = vT[j]->getTarjetaUsuario();
            if( u->edad() > edad && vT[j]->getAviso() == "" ){
                vT[j]->setAviso( _centros[i].getId() + ": Acuda al centro más cercano para vacunarse.");
                _centros[i].anadirTarjetaLista( vT[j] );
                vId.push_back( vT[j]->getId() );
            }
        }  
    }
    return vId;
}

/**
 * @brief Función que vacuna a todos los usuarios cuya id de tarjeta se encuentra en el vector otorgado
 * @param idTarjetas: vector con todas las id de TarjetaUsuario para vacunar a los Usuarios
 * @return número de dosis administradas
 */
int GestionVacunas::vacunarConjuntoTarjetas(std::vector<std::string> idTarjetas){
    
    TarjetaVacunacion *t;
    int cont;
    
    for( int i = 0; i < idTarjetas.size(); i++ ){
        
        t = buscarTarjeta( idTarjetas[i], t);
        if( vacunarUsuario(t) )
            cont++;
    }
    return cont;
}