
#include <vector>
#include <iostream>
#include <string>
#include <ratio>
#include "GestionVacunas.h"

using namespace std;

void linea(){
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

int main() {
    try{      
        GestionVacunas gv("usuarios.txt","dosis.txt","centros.txt");
        gv.estadoCarga();
        std::vector<std::string> vNSS = gv.listadoNSS();
        
         /*Eliminar las tarjetas de vacunación de todos los usuarios cuyo NSS acabe en ‘0’ de la
         tabla hash.*/
        linea();
        std::cout << "Se procede al borrado de las tarjetas cuyos usuarios tenga NSS acabado en 0." << std::endl;
        int cont = 0;
        for( int i = 0; i < vNSS.size(); i++ ){

            if( vNSS[i][9] == '0' ){
                cont++;
                Usuario *u = gv.buscarUsuario(vNSS[i]);
                std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                TarjetaVacunacion *t;
                gv.buscarTarjeta(claveTarjeta,t);
                gv.borrarTarjeta( t );
            }
            
        }
        std::cout << "Se han borrado " << cont << " tarjetas de vacunación. Quedan en el sistema: " <<
                gv.numTarjetas() << std::endl;
        
        /* Añadir las tarjetas de vacunación anteriormente borradas de aquellos usuarios cuyo
        NSS acabe en ‘30’ a la tabla hash.*/
        linea();
        std::cout << "Se procede a la inserción de las tarjetas cuyos usuarios tenga NSS acabado en 30." << std::endl;
        cont = 0;
        for( int i = 0; i < vNSS.size(); i++ ){

            if( vNSS[i][8] == '3' && vNSS[i][9] == '0' ){
                cont++;
                Usuario *u = gv.buscarUsuario(vNSS[i]);
                std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                TarjetaVacunacion t( claveTarjeta,false, gv.centroCercano(u)->getId(),u );
                gv.insertarTarjeta(t);
            }
        }
        std::cout << "Se han insertado " << cont << " tarjetas de vacunación. Hay en el sistema: " <<
                gv.numTarjetas() << std::endl;
        
        /*Mostrar el número de colisiones máximo que se han producido al volver a insertar las
        tarjetas de vacunación.*/
        linea();
        std::cout << "Se han llegado a producir " << gv.colisionesMaximas() << " colisiones." << std::endl;
        
        
        /*Vacunar a todos los usuarios cuyo NSS sea par de la primera dosis y mostrar el
        número de vacunados. Para ello, se debe obtener su tarjeta de vacunación.*/
        cont = 0;
        
        for( int i = 0; i < vNSS.size(); i++ ){
            Usuario *u = gv.buscarUsuario(vNSS[i]);
            if( u != NULL){
                int nss = stoi(vNSS[i]);
                if( nss % 2 == 0 && u->edad() > 12){
                    
                    std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                    TarjetaVacunacion *t = 0;
                    gv.buscarTarjeta(claveTarjeta,t);

                    if( t != 0){
                        if( gv.vacunarUsuario(t) )
                             cont++;
                    }
                }
            }
        }
        std::cout << "Se han vacunado de la primera dosis a " << cont << " personas con NSS par." << std::endl;

        /*Vacunar de la segunda dosis a todos los usuarios que tengan entre 20 y 50 años.*/
        cont = 0;
        
        for( int i = 0; i < vNSS.size(); i++ ){
            Usuario *u = gv.buscarUsuario(vNSS[i]);
            if( u != NULL){
                if( u->edad() >= 20 && u->edad() <= 50 ){
                    
                    std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                    TarjetaVacunacion *t = 0;
                    gv.buscarTarjeta(claveTarjeta,t);
                    if( t != 0){
                        //COMO SE HABLÓ EN CLASE, PARA AUMENTAR LA CANTIDAD DE VACUNAS -> NOS DA IGUAL EL NÚMERO DE VACUNAS ( QUE TENGAN YA O NO )
                        //if( t->getNDosis() == 1 ){
                                if( gv.vacunarUsuario(t) )
                                     cont++;
                                //}
                    } 
                }
            }
        }
        std::cout << "Se han vacunado de la segunda dosis a " << cont << " personas con 1 dosis y edad [20,50]." << std::endl;
        
        /* Vacunar a todos los usuarios cuyo NSS acabe en 2 o 6 que aún no tengan la pauta completa.*/
        cont = 0;

        for( int i = 0; i < vNSS.size(); i++ ){
            
            if( vNSS[i][9] == '2' || vNSS[i][9] == '6'){
                Usuario *u = gv.buscarUsuario(vNSS[i]);
                   
                    std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                    TarjetaVacunacion *t = 0;
                    gv.buscarTarjeta(claveTarjeta,t);
                    if( t != 0){
                            
                        if( !t->isPautaCompleta() ){
                            if( gv.vacunarUsuario(t) )
                               cont++;
                        }
                    } 
                
            }
        }
        std::cout << "Se han vacunado a los usuarios acabados su NSS en 2 o 6: " << cont << " usuarios." << std::endl;
        /* Comprobar cuántos usuarios tienen pasaporte covid.*/
        cont = 0;
        std::vector<std::string> hexCodes;
        for( int i = 0; i < vNSS.size(); i++ ){

                Usuario *u = gv.buscarUsuario(vNSS[i]);
                
                    std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                    TarjetaVacunacion *t = 0;
                    gv.buscarTarjeta(claveTarjeta,t);
                    if( t != 0){
                        if( t->isPautaCompleta() ){
                            hexCodes.push_back(t->pasaporteCovidCode());
                            cont++;
                        }      
                }
        }
        std::cout << "Disponen de pasaporte covid " << cont << " usuarios." << std::endl;
        linea();
        std::cout << "MOSTRAR PASAPORTES COVID -> PULSAR ENTER." << std::endl;
        getchar();
        for( int i = 0; i < hexCodes.size(); i++ )
            std::cout << hexCodes[i] << std::endl;
        
        
    }catch(std::bad_alloc &ba){
        std::cerr << "Excepción bad_alloc recogida: " << ba.what() << std::endl;
    }catch(std::runtime_error &re){
        std::cerr << "Excepción runtime_error recogida: " << re.what() << std::endl;
    }catch(std::out_of_range &oor){
        std::cerr << "Excepción out_of_range recogida: " << oor.what() << std::endl;
    }
    return 0;
}