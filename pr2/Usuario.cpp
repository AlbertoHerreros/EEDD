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
    _nDosis = 0;
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

Usuario& Usuario::administrarVacuna( Dosis *d ){
    _tieneAdministradas = d;
    return(*this);
}

void Usuario::mostrarDosis(){
    
        std::cout << "         --> ";
        _tieneAdministradas->mostrarDosis();
        std::cout << std::endl;
         
}

void Usuario::mostrarUsuario(){
    std::cout <<  "Nombre: " << _nombre << " <-> Apellidos: " << _apellidos << " <-> NSS: " << _NSS << " <-> Fecha nacimiento: " << _fechaNacimiento.cadena() << 
             " <-> Edad: " << edad() << " <-> Dosis administradas: \n";
    //mostrarDosis();
}

bool Usuario::operator==( const Usuario &u ){
    return _nombre == u._nombre;
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

