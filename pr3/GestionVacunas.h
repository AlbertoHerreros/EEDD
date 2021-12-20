/* 
 * File:   GestionVacunas.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 21 de octubre de 2021, 13:03
 */

#ifndef GESTIONVACUNAS_H
#define GESTIONVACUNAS_H

#include <iostream>
#include <fstream>
#include "VDinamico.h"
#include "AVL.h"
#include "Usuario.h"
#include "Dosis.h"

class GestionVacunas {
public:
    GestionVacunas( std::string nomFichUsuarios, std::string nomFichDosis );
    GestionVacunas( const GestionVacunas& orig );
    virtual ~GestionVacunas();
    
    Usuario* buscarUsuario( std::string NSS );
    bool administrarDosis( Usuario *u, Dosis::fabricante fabricante );
    VDinamico<std::string> listadoNSS();
    int pautaCompleta();
    VDinamico<Usuario*> listadoVacunacionNR();
    
    int vacunasAlmacen();
    
    void estadoCarga();
    void mostrarTodasDosis();
    
private:
    AVL<Usuario> _usuarios;
    VDinamico<Dosis> _almacena;
    
};

#endif /* GESTIONVACUNAS_H */

