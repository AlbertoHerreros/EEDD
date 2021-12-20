/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CentroVacunacion.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 10 de noviembre de 2021, 19:35
 */

#ifndef CENTROVACUNACION_H
#define CENTROVACUNACION_H

#include <map>
#include <list>
#include <iostream>
#include "Dosis.h"
#include "Usuario.h"
#include "TarjetaVacunacion.h"

class GestionVacunas;

class CentroVacunacion {
public:
    CentroVacunacion( int id, UTM direccion );
    CentroVacunacion(const CentroVacunacion& orig);
    virtual ~CentroVacunacion();
    
    CentroVacunacion& operator=(const CentroVacunacion &orig);
    
    void alarmaFaltaDosis( Dosis::fabricante fabricante );
    void anadirTarjetaLista( TarjetaVacunacion *t );
    bool administrarDosis( TarjetaVacunacion *t, Dosis::fabricante fabricante );
    void anadirNDosisAlmacen( std::vector<Dosis*> packDosis );
    int numVacunasTipo( Dosis::fabricante f );
    
    float getLatitud();
    float getLongitud();
    
    int getNDosisAlmacenadas();
    void setAlarma(GestionVacunas* _alarma);
    
    int getId() const;
    UTM getUTM();
private:
    int _id;
    UTM _direccion;
    
    multimap<std::string,Dosis> _almacena;
    std::list<TarjetaVacunacion*> _seVacunaAqui; 
    
    GestionVacunas *_alarma;
};

#endif /* CENTROVACUNACION_H */

