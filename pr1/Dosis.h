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
        enum fabricante{Pfizer=0,
                Moderna=1,
                AstraZeneca=2,
                Johnson=3 };
                
        Dosis() = default;
        Dosis( int id, int idLote, fabricante f, Fecha fFabr, Fecha fCad );
        Dosis(const Dosis& orig);
        virtual ~Dosis();
        
        bool operator< ( const Dosis &otra );
        bool operator== ( const Dosis &otra );
        void mostrarDosis();
        
        void SetFechaCaducidad(Fecha _fechaCaducidad);
        Fecha GetFechaCaducidad() const;
        void SetFechaFabricacion(Fecha _fechaFabricacion);
        Fecha GetFechaFabricacion() const;
        void SetFabricante(fabricante _fabricante);
        fabricante GetFabricante() const;
        void SetIdLote(int _idLote);
        int GetIdLote() const;
        void SetId(int _id);
        int GetId() const;
        
private:
        int _id;
        int _idLote;
        fabricante _fabricante;
        Fecha _fechaFabricacion;
        Fecha _fechaCaducidad;
};

#endif /* DOSIS_H */

