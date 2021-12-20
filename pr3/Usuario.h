/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Usuario.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 5 de octubre de 2021, 19:24
 */

#ifndef USUARIO_H
#define USUARIO_H

#include "fecha.h"
#include "Dosis.h"
#include "VDinamico.h"
#include "ListaEnlazada.h"
#include <iostream>

class Usuario {
    private:
        std::string _nombre = "";
        std::string _apellidos = "";
        std::string _NSS = "";
        Fecha _fechaNacimiento = Fecha();
        //UTM _domicilio;
        ListaEnlazada<Dosis*> _tieneAdministradas;
        int _nDosis = 0;
        
    public:
        Usuario() = default;
        Usuario( std::string nombre, std::string apellidos, std::string NSS, Fecha fNac );
        Usuario( const Usuario& orig );
        Usuario& operator=( const Usuario& orig );
        
        virtual ~Usuario();
       
        int edad();
        VDinamico<Dosis*> getDosis();
        Dosis::fabricante getDosisRecomendable();
        void administrarDosis( Dosis *d );
        
        void mostrarUsuario();
        
        bool operator==( const Usuario &u );
        bool operator< ( const Usuario &u );
        bool operator> ( const Usuario &u );
        
        void SetFechaNacimiento(Fecha _fechaNacimiento);
        Fecha GetFechaNacimiento() const;
        void SetNSS(std::string _NSS);
        std::string GetNSS() const;
        void SetApellidos(std::string _apellidos);
        std::string GetApellidos() const;
        void SetNombre(std::string _nombre);
        std::string GetNombre() const;
        void setNDosis(int _nDosis);
        int getNDosis() const;
        
};

#endif /* USUARIO_H */

