/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TarjetaVacunacion.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 23 de noviembre de 2021, 18:54
 */

#include "TarjetaVacunacion.h"

TarjetaVacunacion::TarjetaVacunacion(std::string id, bool pauta, int idCentro, Usuario* u):
_id(id),_pautaCompleta(pauta),_idCentroCercano(idCentro),_tarjetaUsuario(u){
    _nDosis = 0;
}

Usuario* TarjetaVacunacion::getTarjetaUsuario() const {
    return _tarjetaUsuario;
}

int TarjetaVacunacion::getIdCentroCercano() const {
    return _idCentroCercano;
}

bool TarjetaVacunacion::isPautaCompleta() const {
    return _pautaCompleta;
}

std::string TarjetaVacunacion::getId() const {
    return _id;
}

TarjetaVacunacion::TarjetaVacunacion(const TarjetaVacunacion &orig):
_id(orig._id),_pautaCompleta(orig._pautaCompleta),_idCentroCercano(orig._idCentroCercano){
    _tarjetaUsuario = orig._tarjetaUsuario;
    _tieneAdministradas = orig._tieneAdministradas;
    _nDosis = orig._nDosis;
}

TarjetaVacunacion& TarjetaVacunacion::operator=( const TarjetaVacunacion &orig ){
    _id = orig._id;
    _pautaCompleta = orig._pautaCompleta;
    _idCentroCercano = orig._idCentroCercano;
    _tarjetaUsuario = orig._tarjetaUsuario;
    _tieneAdministradas = orig._tieneAdministradas;
    _nDosis = orig._nDosis;
    return *this;
}

TarjetaVacunacion::~TarjetaVacunacion(){
}

std::vector<Dosis*> TarjetaVacunacion::getDosis(){
    return _tieneAdministradas;
}

std::string TarjetaVacunacion::pasaporteCovidCode(){
    if( _pautaCompleta ){
        Dosis::fabricante f = _tieneAdministradas[0]->getFabricante();
        std::string fab = to_string(f);

        std::string str = _id + fab;
        str += to_string(_nDosis);
        std::string hash_hex;
        picosha2::hash256_hex_string(_id,hash_hex);
        
        return hash_hex;
        }
    return "not found";
}

void TarjetaVacunacion::administrarDosis( Dosis *d ){
    _tieneAdministradas.push_back(d);
    _nDosis++;
}

int TarjetaVacunacion::getNDosis() const{
    return _nDosis;
}

void TarjetaVacunacion::setPautaCompleta(bool _pautaCompleta) {
    this->_pautaCompleta = _pautaCompleta;
}