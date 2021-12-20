/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   THashTarjetaVacunacion.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 23 de noviembre de 2021, 19:33
 */

#ifndef THASHTARJETAVACUNACION_H
#define THASHTARJETAVACUNACION_H

#include "TarjetaVacunacion.h"
#include <iostream>
#include <vector>

class THashTarjetaVacunacion {
    
private:
    //Estado de cada casilla de la THash
    enum Estado{
        empty = 0,
        taken = 1,
        deleted = 2
    };
    //Clase encargada de definir cada uno de los datos del <vector> de la THash
    class Casilla{
        public:
            TarjetaVacunacion _tarjeta;
            Estado _estado;

            Casilla():_estado(empty){}
            ~Casilla(){}
    };
    
    
    
    int hash(unsigned long clave, int intento);

    std::vector<Casilla> _vector;
    
    int _tamTabla;
    int _nDatos;    
    int _nColisiones;
    int _maxColisiones;
    int _ocurrenMasDiezColisiones;
    
    int sigPrimo( int n );
    
public:
    unsigned long djb2(std::string str);
    
    THashTarjetaVacunacion( int tam );
    THashTarjetaVacunacion( const THashTarjetaVacunacion &orig );
    THashTarjetaVacunacion& operator=( THashTarjetaVacunacion &orig );
    virtual ~THashTarjetaVacunacion();
    
    bool insertar( unsigned long clave, TarjetaVacunacion &tarjeta );
    bool buscar( unsigned long clave, std::string &id, TarjetaVacunacion *&tarjeta );
    bool borrar( unsigned long clave, std::string &id );
    
    unsigned int numTarjetas();
    
    unsigned int maxColisiones();
    unsigned int numMax10();
    float promedioColisiones();
    float factorCarga();
    unsigned int tamTabla();
    
    void informeGeneral(){
        std::cout << "\n--- INFORME SOBRE THASH ---" << std::endl;
        std::cout << "Tamaño de tabla: " << _tamTabla << std::endl;
        std::cout << "Número de datos que alberga: " << _nDatos << std::endl;
        std::cout << "Número máximo de colisiones: " << _maxColisiones << std::endl;
        std::cout << "Ocurrem más de 10 colisiones: " << _ocurrenMasDiezColisiones << std::endl;
    }
    
};

#endif /* THASHTARJETAVACUNACION_H */

