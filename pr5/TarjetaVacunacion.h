/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TarjetaVacunacion.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 23 de noviembre de 2021, 18:54
 */

#ifndef TARJETAVACUNACION_H
#define TARJETAVACUNACION_H

#include <vector>
#include <iostream>

#include "Dosis.h"
#include "Usuario.h"
#include "picosha2.h"

class TarjetaVacunacion {
public:
    TarjetaVacunacion() = default;
    TarjetaVacunacion( std::string id, bool pauta, int idCentro, Usuario *u );
    TarjetaVacunacion(const TarjetaVacunacion &orig);
    TarjetaVacunacion& operator=( const TarjetaVacunacion &orig);
    virtual ~TarjetaVacunacion();
    
    std::vector<Dosis*> getDosis();
    std::string pasaporteCovidCode();
    Dosis::fabricante getDosisRecomendable(){ return _tarjetaUsuario->getDosisRecomendable(); }
    UTM getDomicilio(){ return _tarjetaUsuario->getUTM(); }
    
    int getIdCentroCercano() const;
    bool isPautaCompleta() const;
    std::string getId() const;
    
    void setUsuario( Usuario* u){ _tarjetaUsuario = u; }
    
    void administrarDosis( Dosis *d );
    Usuario* getTarjetaUsuario() const;
    int getNDosis() const;
    void setPautaCompleta(bool _pautaCompleta);
    
private:
    std::string _id = "Error";
    bool _pautaCompleta = false; //Se actualiza en administrarDosis si se cumple condición
    int _idCentroCercano = 0;
    Usuario *_tarjetaUsuario = 0;
    std::vector<Dosis*> _tieneAdministradas;
    
    int _nDosis;
};

#endif /* TARJETAVACUNACION_H */

