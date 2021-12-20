/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   THashTarjetaVacunacion.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 23 de noviembre de 2021, 19:33
 */

#include <math.h>

#include "THashTarjetaVacunacion.h"

THashTarjetaVacunacion::THashTarjetaVacunacion( int tam ) {
    
    _tamTabla = sigPrimo( tam );
    _vector = std::vector<Casilla>(_tamTabla);
    _nDatos = 0;
    _nColisiones = 0;
    _maxColisiones = 0;
    _ocurrenMasDiezColisiones = 0;

}

THashTarjetaVacunacion::THashTarjetaVacunacion(const THashTarjetaVacunacion& orig){
    _vector = orig._vector;
    _tamTabla = orig._tamTabla;
    _nDatos = orig._nDatos;
    _nColisiones = orig._nColisiones;
    _maxColisiones = orig._maxColisiones;
    _ocurrenMasDiezColisiones = orig._ocurrenMasDiezColisiones;
}

THashTarjetaVacunacion& THashTarjetaVacunacion::operator=( THashTarjetaVacunacion &orig ){
    _vector = orig._vector;
    _tamTabla = orig._tamTabla;
    _nDatos = orig._nDatos;
    _nColisiones = orig._nColisiones;
    _maxColisiones = orig._maxColisiones;
    _ocurrenMasDiezColisiones = orig._ocurrenMasDiezColisiones;
    return *this;
}

THashTarjetaVacunacion::~THashTarjetaVacunacion() {
}

unsigned long THashTarjetaVacunacion::djb2(std::string str) {
    unsigned long hash = 5381;
    int c;
    int i = 0;
    for( int i = 0; i < str.length(); i++ ){
        c = (int) str[i];
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
int THashTarjetaVacunacion::hash(unsigned long clave, int intento){

    
    //h1(x)
//    int primo = 7079;
//    return ( ( clave % _tamTabla ) + ( intento * ( ( clave % primo ) + 1 ) ) ) % _tamTabla;
    
    //h2(x)
    int primo2 = 997;
    return ( ( clave % _tamTabla ) + ( intento * ( primo2 - ( clave % primo2 ) + 1 ) ) ) % _tamTabla;
    
    //h3(x)
  //  return ( clave + intento * intento) % _tamTabla ;
}

int THashTarjetaVacunacion::sigPrimo( int n ){
    bool esPrimo = false;
    while( !esPrimo ){
        n++;
        bool comprobar = true;
        for( int i = 2; i <= sqrt(n) && comprobar; i++)
            if( n % i == 0 )
                comprobar = false;
        
        if( comprobar )
            return n;
    }
    return -1;
}

bool THashTarjetaVacunacion::insertar( unsigned long clave, TarjetaVacunacion &tarjeta ){
    
    TarjetaVacunacion *t = 0;
    std::string id = tarjeta.getId();
    
    if( buscar( 0,id,t ) ){
        return false;
    }
    else{
        
        int intento = 0;
        int pos = hash( djb2( id ), intento );
        Casilla *c = &_vector[pos];
        
        while( c->_estado == taken ){
            intento++;
            pos = hash( djb2(id), intento );
            c = &_vector[pos];
        }
        
        if( intento > 10 )
            _ocurrenMasDiezColisiones++;
        
        if( intento > _maxColisiones )
            _maxColisiones = intento;
       
        _nColisiones += intento;
        _nDatos++;
        
        c->_tarjeta = tarjeta;
        c->_estado = taken;
            return true;
    }
}

bool THashTarjetaVacunacion::buscar( unsigned long clave, std::string &id, TarjetaVacunacion *&tarjeta ){
    
    int intento = 0;
    int pos = hash( djb2(id), intento );
    Casilla *c = &_vector[pos];
    
    while( c->_estado == taken || c->_estado == deleted ){
        if( c->_estado == taken ){
            if( id == c->_tarjeta.getId() ){
                tarjeta = &c->_tarjeta;
                return true;
            }
        }
        intento++;
        pos = hash( djb2(id), intento);
        c = &_vector[pos];
    }
    return false;
}

bool THashTarjetaVacunacion::borrar( unsigned long clave, std::string &id ){
    
    TarjetaVacunacion *t = 0;
    
    if( !buscar( 0,id,t ) ){
        return false;
    }
    else{
        
        int intento = 0;
        int pos = hash( djb2( id ), intento );
        Casilla *c = &_vector[pos];
        
        while( c->_estado == taken || c->_estado == deleted ){
            if( c->_estado == taken && id == c->_tarjeta.getId() ){
                c->_estado = deleted;
                c->_tarjeta = TarjetaVacunacion();
                _nDatos--;
                return true;
            }
            intento++;
            pos = hash( djb2(id), intento );
            c = &_vector[pos];
        }
        return false;
    }
}

/**
 * @brief Función que devuelve el total de datos existentes en la THash
 * @return nDatos que existen
 */
unsigned int THashTarjetaVacunacion::numTarjetas(){
    return _nDatos;
}

/**
 * @brief Función que devuelve el máximo de colisiones registradas en una operación de la THash
 * @return máximo de colisiones registradas 
 */
unsigned int THashTarjetaVacunacion::maxColisiones(){
    return _maxColisiones;
}

/**
 * @brief Función que devuelve el número de veces que se sobrepasan las 10 colisiones en una operación
 * @return n veces que ocurre
 */
unsigned int THashTarjetaVacunacion::numMax10(){
    return _ocurrenMasDiezColisiones;
}
/**
 * @brief Función que calcula el promedio de colisiones que ocurren en la THash
 * @return promedio de colisiones ( total colisiones / datos introducidos )
 */
float THashTarjetaVacunacion::promedioColisiones(){
    float colisiones = _nColisiones;
    float datos = _nDatos;
    
    return colisiones/datos;
}
/**
 * @brief Función que calcula el factor de carga actual de la THash
 * @return factor de carga ( datos introducidos / tamaño físico de la THash)
 */
float THashTarjetaVacunacion::factorCarga(){
    float datos = _nDatos;
    float tamT = _tamTabla;
    
    return datos/tamT;
}

/**
 * @brief Función que devuelve el tamaño físico de la THash
 * @return tamaño total de la THash
 */
unsigned int THashTarjetaVacunacion::tamTabla(){
    return _tamTabla;
}