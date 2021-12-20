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
Dosis::Dosis(int id, int idLote, fabricante f, Fecha fFabr, Fecha fCad):
_id(id),_idLote(idLote),_fabricante(f),_fechaFabricacion(fFabr),_fechaCaducidad(fCad){
}

Dosis::Dosis(const Dosis& orig):
_id(orig._id),_idLote(orig._idLote),_fabricante(orig._fabricante),_fechaFabricacion(orig._fechaFabricacion),_fechaCaducidad(orig._fechaCaducidad){
}

Dosis::~Dosis() {
}

bool Dosis::operator <(const Dosis& otra){
    return _id < otra._id;
}

bool Dosis::operator ==(const Dosis& otra){
    return _id == otra._id;
}

void Dosis::mostrarDosis(){
    std::cout << "ID: " << _id << " |/| IDLOTE: " << _idLote <<  "  |/| FABRICANTE: " << _fabricante << " |/| FECHA FABRICACIÓN: " <<
            _fechaFabricacion.cadena() << " |/| FECHA CADUCIDAD: " << _fechaCaducidad.cadena() << std::endl;
}

void Dosis::SetFechaCaducidad(Fecha _fechaCaducidad) {
    this->_fechaCaducidad = _fechaCaducidad;
}

Fecha Dosis::GetFechaCaducidad() const {
    return _fechaCaducidad;
}

void Dosis::SetFechaFabricacion(Fecha _fechaFabricacion) {
    this->_fechaFabricacion = _fechaFabricacion;
}

Fecha Dosis::GetFechaFabricacion() const {
    return _fechaFabricacion;
}

void Dosis::SetFabricante(fabricante _fabricante) {
    this->_fabricante = _fabricante;
}



void Dosis::SetIdLote(int _idLote) {
    this->_idLote = _idLote;
}

int Dosis::GetIdLote() const {
    return _idLote;
}

void Dosis::SetId(int _id) {
    this->_id = _id;
}

int Dosis::GetId() const {
    return _id;
}
