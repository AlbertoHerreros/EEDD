/* 
 * File:   GestionVacunas.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 21 de octubre de 2021, 13:03
 */

#ifndef GESTIONVACUNAS_H
#define GESTIONVACUNAS_H

#include <vector>
#include <map>

#include <iostream>
#include <fstream>
#include "CentroVacunacion.h"
#include "Usuario.h"
#include "Dosis.h"


class GestionVacunas {
public:
    GestionVacunas( std::string nomFichUsuarios, std::string nomFichDosis, std::string nomFichCentros );
    GestionVacunas( const GestionVacunas& orig );
    virtual ~GestionVacunas();
    
    Usuario* buscarUsuario( std::string NSS );
    std::vector<std::string> listadoNSS();
    int numTotalVacunasTipo( Dosis::fabricante f );

    int pautaCompleta();
    std::vector<Usuario*> listadoVacunacionNR();
    CentroVacunacion* vacunarUsuario( Usuario *u );
    
    void suministrarNDosisAlCentro( CentroVacunacion *centro, int nDosis );
    
    int vacunasAlmacen();
    
    void estadoCarga();
    
private:
    std::map<std::string,Usuario> _usuarios;
    std::vector<CentroVacunacion> _centros;
    std::string _nomFichDosis;
    
    int _lineaDosis; //Línea de lectura del fichero ( por donde va )
    
};

#endif /* GESTIONVACUNAS_H */

