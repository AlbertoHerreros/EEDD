/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dosis.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 27 de septiembre de 2021, 17:52
 */

#include "Dosis.h"
Dosis::Dosis(int id, int idLote, fabricante f, estado e, Fecha fFabr, Fecha fCad):
_id(id),_idLote(idLote),_fabricante(f),_estado(e),_fechaFabricacion(fFabr),_fechaCaducidad(fCad){
    _estado = Dosis::enAlmacen;
}

Dosis::Dosis(const Dosis& orig):
_id(orig._id),_idLote(orig._idLote),_fabricante(orig._fabricante),_estado(orig._estado),_fechaFabricacion(orig._fechaFabricacion),_fechaCaducidad(orig._fechaCaducidad){
}

Dosis::~Dosis() {
}

Dosis& Dosis::operator=(const Dosis& orig){
    if( this != &orig){
        _id = orig._id;
        _idLote = orig._idLote;
        _fabricante = orig._fabricante;
        _fechaFabricacion = orig._fechaFabricacion;
        _fechaCaducidad = orig._fechaCaducidad;
        _estado = orig._estado;
    }
    return (*this);
}

bool Dosis::operator <(const Dosis& otra){
    return _fechaFabricacion < otra._fechaFabricacion;
}

bool Dosis::operator ==(const Dosis& otra){
    return _id == otra._id;
}

void Dosis::mostrarDosis(){
    std::cout << "ID: " << _id << " <-> IDLOTE: " << _idLote <<  "  <-> FABRICANTE: " << _fabricante << " <-> ESTADO: " << _estado << " <-> FECHA FABRICACIÓN: " <<
            _fechaFabricacion.cadena() << " <-> FECHA CADUCIDAD: " << _fechaCaducidad.cadena() << std::endl;
}

void Dosis::setFechaCaducidad(Fecha _fechaCaducidad) {
    this->_fechaCaducidad = _fechaCaducidad;
}

Fecha Dosis::getFechaCaducidad() const {
    return _fechaCaducidad;
}

void Dosis::setFechaFabricacion(Fecha _fechaFabricacion) {
    this->_fechaFabricacion = _fechaFabricacion;
}

Fecha Dosis::getFechaFabricacion() const {
    return _fechaFabricacion;
}

void Dosis::setEstado(estado _estado) {
    this->_estado = _estado;
}

Dosis::estado Dosis::getEstado() const {
    return _estado;
}

void Dosis::setFabricante(fabricante _fabricante) {
    this->_fabricante = _fabricante;
}

Dosis::fabricante Dosis::getFabricante() const {
    return _fabricante;
}

void Dosis::setIdLote(int _idLote) {
    this->_idLote = _idLote;
}

int Dosis::getIdLote() const {
    return _idLote;
}

void Dosis::setId(int _id) {
    this->_id = _id;
}

int Dosis::getId() const {
    return _id;
}

