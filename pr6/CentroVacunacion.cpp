/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CentroVacunacion.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 10 de noviembre de 2021, 19:35
 */

#include "CentroVacunacion.h"
#include "GestionVacunas.h"

CentroVacunacion::CentroVacunacion( int id, UTM direccion):
_id(id),_direccion(direccion){
}

CentroVacunacion::CentroVacunacion(const CentroVacunacion& orig):
_id(orig._id),_direccion(orig._direccion),_alarma(orig._alarma),_almacena(orig._almacena),
        _seVacunaAqui(orig._seVacunaAqui){
}

CentroVacunacion::~CentroVacunacion() {
}

CentroVacunacion& CentroVacunacion::operator=(const CentroVacunacion &orig){
    _id = orig._id;
    _direccion = orig._direccion;
    _alarma = orig._alarma;
    _seVacunaAqui = orig._seVacunaAqui;
    _almacena = orig._almacena;
    return *this;
}

void CentroVacunacion::alarmaFaltaDosis( Dosis::fabricante fabricante ){
    _alarma->suministrarNDosisAlCentro(this,100);
}

void CentroVacunacion::anadirTarjetaLista( TarjetaVacunacion *t ){
    _seVacunaAqui.push_back(t);
}

bool CentroVacunacion::administrarDosis( TarjetaVacunacion *t, Dosis::fabricante fabricante ){
    
    //Si no tiene aviso, se vacunará en caso de que sea mayor de 12
    if(t->getAviso() == "" )
        if( fabricante == Dosis::null )
            return false;
        //Buscar Usuario en el CentroVacunacion
        bool comprobar = false;
        //¿Quedan Dosis?
        bool aunQuedan = false;
        std::list<TarjetaVacunacion*>::iterator busca = _seVacunaAqui.begin();
        std::list<TarjetaVacunacion*>::iterator borradoTarjeta;
        while( busca != _seVacunaAqui.end() ){
            if( (*busca)->getId() == t->getId()){
                comprobar = true;
                borradoTarjeta = busca;
            }
            ++busca;
        }
        //Si está el Usuario en el CentroVacunación
        if( comprobar){
        //Se busca la dosis recomendable
        std::multimap<std::string,Dosis>::iterator it;
        for( it = _almacena.begin(); it != _almacena.end(); it++ ){
            
            if( (*it).second.getEstado() == Dosis::enAlmacen ){ //¿Almacén vacío?
                aunQuedan = true;
                if( fabricante == (*it).second.getFabricante() ){ //==Fabricante
                    //Administra dosis, estado dosis -> administrada, se borra Usuario lista CentroVacunacion
                    
                    Dosis *d = &(it->second);
                    t->administrarDosis( d );
                    d->setEstado( Dosis::administrada );
                    
                    t->setAviso("");
                    //¿PAUTA COMPLETA?
                    if( t->getTarjetaUsuario()->edad() < 75 && t->getNDosis() == 2 )
                        t->setPautaCompleta(true);
                    else if( t->getTarjetaUsuario()->edad() >= 75 && t->getNDosis() == 3)
                        t->setPautaCompleta(true);
                        
                    _seVacunaAqui.erase(borradoTarjeta);
                    return true;
                }  
            }
        }
        
        //No hay dosis --> ALARMA        
        if( aunQuedan == false )
            alarmaFaltaDosis(fabricante);

        //Comprobamos que en la solicitud de vacunas se haya entregado alguna recomendada
        for( it = _almacena.begin(); it != _almacena.end(); it++ ){

            if( fabricante == (*it).second.getFabricante() && (*it).second.getEstado() == Dosis::enAlmacen ){ //==Fabricante y en almacén
                //Administra dosis, estado dosis -> administrada, se borra Usuario lista CentroVacunacion
                Dosis *d = &(it->second);
                t->administrarDosis( d );
                d->setEstado( Dosis::administrada );
                
                t->setAviso("");
                //¿PAUTA COMPLETA?
                if( t->getTarjetaUsuario()->edad() < 75 && t->getNDosis() == 2 )
                    t->setPautaCompleta(true);
                else if( t->getTarjetaUsuario()->edad() >= 75 && t->getNDosis() == 3)
                    t->setPautaCompleta(true);
                
                _seVacunaAqui.erase(borradoTarjeta);
                return true;
            }  
        }
        //Si no quedan dosis recomendables, se le administra otra distinta que haya en el almacén
        for( it = _almacena.begin(); it != _almacena.end(); it++ ){

            if( (*it).second.getEstado() == Dosis::enAlmacen ){
                Dosis *d = &(it->second);
                t->administrarDosis( d );
                d->setEstado( Dosis::administrada );
                
                t->setAviso("");
                //¿PAUTA COMPLETA?
                if( t->getTarjetaUsuario()->edad() < 75 && t->getNDosis() == 2 )
                    t->setPautaCompleta(true);
                else if( t->getTarjetaUsuario()->edad() >= 75 && t->getNDosis() == 3)
                    t->setPautaCompleta(true);
                
                _seVacunaAqui.erase(borradoTarjeta);
                return true;
            }
        }
    }
    return false;
}


void CentroVacunacion::anadirNDosisAlmacen( std::vector<Dosis*> packDosis ){
   
    std::pair<std::string,Dosis> par;
    
    for( int i = 0; i < packDosis.size(); i++ ){
        
        par.first = packDosis[i]->getFabricante();
        par.second = *packDosis[i];
        _almacena.insert(par);

    }   
}

int CentroVacunacion::numVacunasTipo(Dosis::fabricante f){
    
    int cont = 0;
    
    std::multimap<std::string,Dosis>::iterator it;
    for( it = _almacena.begin(); it != _almacena.end(); ++it ){
        
        if( (*it).second.getFabricante() == f && (*it).second.getEstado() == Dosis::enAlmacen )
            cont++;
    }
    return cont;
}

float CentroVacunacion::getLatitud(){
    return _direccion.lat;
}

float CentroVacunacion::getLongitud(){
    return _direccion.lon;
}

int CentroVacunacion::getNDosisAlmacenadas(){
    int cont = 0;
    std::multimap<std::string,Dosis>::iterator it;
    for( it = _almacena.begin(); it != _almacena.end(); ++it )
        if( (*it).second.getEstado() == Dosis::enAlmacen )
            cont++;
    return cont;
}

void CentroVacunacion::setAlarma(GestionVacunas* _alarma) {
    this->_alarma = _alarma;
}

int CentroVacunacion::getId() const {
    return _id;
}

UTM CentroVacunacion::getUTM(){
    return _direccion;
}










