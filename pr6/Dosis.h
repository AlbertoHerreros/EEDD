/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dosis.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 27 de septiembre de 2021, 17:52
 */

#ifndef DOSIS_H
#define DOSIS_H

#include <iostream>
#include "Fecha.h"

class Dosis {

    public:
        //Fabricante de la vacuna
        enum fabricante{Pfizer=0,
                Moderna=1,
                AstraZeneca=2,
                Johnson=3,
                null=4}; // null es caso especial si el Usuario tiene edad <= 12 años
        
        //Estado de la dosis
        enum estado{
            enAlmacen=0,
            administrada=1};
                
        Dosis() = default;
        Dosis( int id, int idLote, fabricante f, estado e, Fecha fFabr, Fecha fCad );
        Dosis(const Dosis& orig);
        virtual ~Dosis();
        
        Dosis& operator=( const Dosis &orig );
        
        bool operator< ( const Dosis &otra );
        bool operator== ( const Dosis &otra );
        void mostrarDosis();
        void setFechaCaducidad(Fecha _fechaCaducidad);
        Fecha getFechaCaducidad() const;
        void setFechaFabricacion(Fecha _fechaFabricacion);
        Fecha getFechaFabricacion() const;
        void setEstado(estado _estado);
        Dosis::estado getEstado() const;
        void setFabricante(fabricante _fabricante);
        Dosis::fabricante getFabricante() const;
        void setIdLote(int _idLote);
        int getIdLote() const;
        void setId(int _id);
        int getId() const;

private:
        int _id;
        int _idLote;
        fabricante _fabricante;
        estado _estado;
        Fecha _fechaFabricacion;
        Fecha _fechaCaducidad;
};

#endif /* DOSIS_H */

