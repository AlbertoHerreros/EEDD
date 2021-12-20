/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Usuario.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 5 de octubre de 2021, 19:24
 */

#include "Usuario.h"

Usuario::Usuario(std::string nombre, std::string apellidos, std::string NSS, Fecha fNac):
_nombre(nombre),_apellidos(apellidos),_NSS(NSS),_fechaNacimiento(fNac){
    _nDosis = 0;
}

Usuario::Usuario(const Usuario& orig):
_nombre(orig._nombre),_apellidos(orig._apellidos),_NSS(orig._NSS),_fechaNacimiento(orig._fechaNacimiento){
    _nDosis = orig._nDosis;
    _tieneAdministradas = orig._tieneAdministradas;
}

Usuario& Usuario::operator=( const Usuario& orig ){
    _nombre = orig._nombre;
    _apellidos = orig._apellidos;
    _NSS = orig._NSS;
    _fechaNacimiento = orig._fechaNacimiento;
    _tieneAdministradas = orig._tieneAdministradas;
    _nDosis = orig._nDosis;
    return *this;
}

Usuario::~Usuario() {
}

int Usuario::edad(){
    Fecha fActual;
    
    if( fActual.verDia() < _fechaNacimiento.verDia() || fActual.verMes() < _fechaNacimiento.verMes() )
        return fActual.verAnio() - _fechaNacimiento.verAnio() - 1;
    else
        return fActual.verAnio() - _fechaNacimiento.verAnio();
}

VDinamico<Dosis*> Usuario::getDosis(){
    VDinamico<Dosis*> v;
    ListaEnlazada<Dosis*>::Iterador<Dosis*> it = _tieneAdministradas.iterador();
    while( !it.fin() ){
        v.insertar( it.dato() );
        it.siguiente();
    }
    return v;
}

Dosis::fabricante Usuario::getDosisRecomendable(){
    
    int edad = this->edad();
        
    if( edad <= 12 )
        return Dosis::null;
    else if( edad > 12 && edad <= 30 )
        return Dosis::Johnson;
    else if( edad > 30 && edad <= 50 )
        return Dosis::Moderna;
    else if( edad > 50 && edad <= 65 )
        return Dosis::AstraZeneca;
    
    return Dosis::Pfizer;
}

void Usuario::administrarDosis( Dosis *d ){
    _tieneAdministradas.insertaFinal(d);
    _nDosis++;
}

void Usuario::mostrarUsuario(){
    std::cout <<  "Nombre: " << _nombre << " <-> Apellidos: " << _apellidos << " <-> NSS: " << _NSS << " <-> Fecha nacimiento: " << _fechaNacimiento.cadena() << 
             " <-> Edad: " << edad() << " <-> Dosis administradas: " << _nDosis << std::endl;
}

bool Usuario::operator==( const Usuario &u ){
    if( std::stoi(this->_NSS) == std::stoi(u._NSS) )
        return true;
    return false;
}

bool Usuario::operator<( const Usuario &u ){
    if( std::stoi(this->_NSS) < std::stoi(u._NSS) )
        return true;
    return false;
}

bool Usuario::operator>( const Usuario &u ){
    if( std::stoi(this->_NSS) > std::stoi(u._NSS) )
        return true;
    return false;
}

void Usuario::SetFechaNacimiento(Fecha _fechaNacimiento) {
    this->_fechaNacimiento = _fechaNacimiento;
}

Fecha Usuario::GetFechaNacimiento() const {
    return _fechaNacimiento;
}

void Usuario::SetNSS(std::string _NSS) {
    this->_NSS = _NSS;
}

std::string Usuario::GetNSS() const {
    return _NSS;
}

void Usuario::SetApellidos(std::string _apellidos) {
    this->_apellidos = _apellidos;
}

std::string Usuario::GetApellidos() const {
    return _apellidos;
}

void Usuario::SetNombre(std::string _nombre) {
    this->_nombre = _nombre;
}

std::string Usuario::GetNombre() const {
    return _nombre;
}

void Usuario::setNDosis(int _nDosis) {
    this->_nDosis = _nDosis;
}

int Usuario::getNDosis() const {
    return _nDosis;
}

