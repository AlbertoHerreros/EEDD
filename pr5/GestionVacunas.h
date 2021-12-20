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
#include "THashTarjetaVacunacion.h"
#include "TarjetaVacunacion.h"
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
    CentroVacunacion* centroCercano( Usuario *u );
    bool vacunarUsuario( TarjetaVacunacion *t );
    
    void suministrarNDosisAlCentro( CentroVacunacion *centro, int nDosis );
    
    int vacunasAlmacen();
    void estadoCarga();
    
    void insertarTarjeta( TarjetaVacunacion &t );
    TarjetaVacunacion* buscarTarjeta( std::string clave, TarjetaVacunacion *&t );
    void borrarTarjeta( TarjetaVacunacion *t );
    unsigned int numTarjetas(){ return _tarjetas.numTarjetas(); }
    
    unsigned int colisionesMaximas(){
        return _tarjetas.maxColisiones();
    }
    
private:
    std::map<std::string,Usuario> _usuarios;
    std::vector<CentroVacunacion> _centros;
      
    //tam1
    THashTarjetaVacunacion _tarjetas = THashTarjetaVacunacion(15384);
    //tam2
    //THashTarjetaVacunacion _tarjetas = THashTarjetaVacunacion(14705);
    
    std::string _nomFichDosis;
    int _lineaDosis; //Línea de lectura del fichero dosis.txt ( por donde va )
    
};

#endif /* GESTIONVACUNAS_H */

