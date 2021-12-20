#include <iostream>
#include <string>
#include <vector>
#include "AVL.h"

#include "VDinamico.h"
#include "GestionVacunas.h"

using namespace std;

void linea(){
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

int main() {
    try{
        /*Crear un árbol AVL instanciado a 1000 enteros aleatorios y comprobar que todas las
         funciones trabajan correctamente.*/
        /*Indicar la altura del árbol AVL.*/
          
        AVL<int> avlEnteros;
        int n, r;
        bool c;
        
        for( int i = 0; i <= 999; i++ ){
            n = rand() % 100000;
            c = avlEnteros.buscaNR(n,r);
            //Comprobar si un número aleatorio ya estaba
            if( !c )
                avlEnteros.inserta( n );
            else
                i--;
        }
        //avlEnteros.recorreInorden();
        linea();
        std::cout << "PRUEBA CON ENTEROS \nNúmero de elementos: " << avlEnteros.numElementos() << std::endl <<
                "Altura del AVL de enteros: " << avlEnteros.altura() << std::endl;
        linea();
        /*Instanciar el árbol con objetos de tipo Usuario con el fichero adjunto. La clase
        GestionVacunas toma el nombre del fichero en el constructor.
        Instanciar el vector de dosis al fichero de dosis adjunto con 30.000 dosis. La clase
        GestionVacunas toma el nombre del fichero en el constructor.*/
        
        GestionVacunas gv("usuarios2.txt","dosis2.txt");
        gv.estadoCarga();
        
        int x = gv.vacunasAlmacen();
        std::cout << "Vacunas sin administrar: " << x << std::endl;;
        linea();
        /*Obtener los NSS de los usuarios en un vector dinámico Vdinamico<String>
        vecAuxilia obtenido con la función GesionVacunas::listadoCompletoNSS(). Este vector
        se utilizará para las funciones que aparecen a continuación.*/
        
        VDinamico<std::string> vecAuxiliar;
        vecAuxiliar = gv.listadoNSS();
        std::cout << "vecAuxiliar dispone de  " << vecAuxiliar.tamLog() << " elementos(NSS)." << std::endl; 
        linea();
        
//        for( int i = 0; i < 9999; i++)
//            std::cout << vecAuxiliar[i] << std::endl;
          
        /*Vacunar de la primera dosis de forma automática a todos los usuarios menos a los que
        su NSS acabe en 0. Para ello se recorre vecAuxiliar, que tiene los NSS de todos los
        usuarios y se buscan dichas claves en el AVL<Usuario>. Diseñar un algoritmo para
        realizar esta asignación. Tened especial cuidado de no suministrar más de una vez una
        dosis (cuando se administra una dosis el estado cambia a administrada). Imprimir el
        número de dosis administradas.*/
        
        Usuario *u;
        int cont = 0;
        bool vacunado = false;
        
        for( int i = 0; i < vecAuxiliar.tamLog(); i++ ){
            //Si último carácter de NSS != 0 -> buscamos al usuario y le administramos su dosis recomendable( si se puede )
            if( vecAuxiliar[i][9] != '0'){
                u = gv.buscarUsuario( vecAuxiliar[i] );
                vacunado = gv.administrarDosis( u, u->getDosisRecomendable() );
                if( vacunado )
                    cont++;
            } 
        }
        
        std::cout << "Se han administrado dosis excepto a los Usuarios cuyos NSS eran XXXXXXXX0 un total " <<
                "de " << cont << " dosis." << std::endl;
        
        /*Ahora, usando el mismo proceso anterior, vacunar de la segunda dosis a todos los
        usuarios menos a aquellos cuyo NSS acabe en 3 (y tampoco obviamente a los que
        acaben en 0). Imprimir el número de dosis administradas.*/
        cont = 0;
        for( int i = 0; i < vecAuxiliar.tamLog(); i++ ){
            //Si último carácter de NSS != 0 && != 3 -> buscamos al usuario y le administramos su dosis recomendable( si se puede )
            if( vecAuxiliar[i][9] != '0' && vecAuxiliar[i][9] != '3'){
                u = gv.buscarUsuario( vecAuxiliar[i] );
                vacunado = gv.administrarDosis( u, u->getDosisRecomendable() );
                if( vacunado )
                    cont++;
            } 
        }
        std::cout << "Se han administrado dosis excepto a los Usuarios cuyos NSS eran XXXXXXXX0 o XXXXXXXX3 un total " <<
                "de " << cont << " dosis." << std::endl;
        
        /*De igual forma, vacunar a todos los usuarios mayores de 75 años con la tercera dosis.
        Para saber la edad, buscar al usuario (GestionVacunas::buscarUsuario()). Si no
        estaban antes vacunados, no están vacunados de las dos anteriores no se vacunan de la
        tercera. Imprimir el número de dosis administradas.*/
        cont = 0;
        for( int i = 0; i < vecAuxiliar.tamLog(); i++ ){
            
            u = gv.buscarUsuario( vecAuxiliar[i] );
            if( u->edad() >= 75 && u->getNDosis() >= 2 ){ //Edad >= 75 && número de dosis administradas == 2
                gv.administrarDosis( u, u->getDosisRecomendable() );
                cont++;
            } 
        }
        std::cout << "Se han administrado dosis a las personas mayores de 75 que tenían 2 dosis previamente un total de " << cont << " dosis." << std::endl;
        
        /*Indicad el porcentaje de personas con pauta completa, es decir, todos los menores de
        75 con 2 dosis y los de 75 con 3 dosis con la función
        GestionVacunas::pautacompleta(). Para ello es conveniente hacer uso de la función
        AVL::recorreInorden()*/
        linea();
        std::cout << "Porcentaje de personas con la pauta de vacunación completa: " << gv.pautaCompleta() << std::endl;
        linea();
        
        /* Listar quienes no han sido vacunados con la pauta recomendada
        GestionVacunas::listadoVacunacionNR(); Para ello es conveniente hacer uso de la
        función AVL::recorreInorden()*/
        
        //std::cout << "Para continuar con el listadoVacunacionNR pulsa enter: " << std::endl;
        //getchar();
        
        VDinamico<Usuario*> vNR;
        vNR = gv.listadoVacunacionNR();
        std::cout << "Lista de personas que no han sido vacunados con la pauta recomendada: " << std::endl;
        for( int i = 0; i < vNR.tamLog(); i++ ){
            std::cout << i+1 << "   -->   ";
            vNR[i]->mostrarUsuario();
        }
        std::cout << "\nHan sido un total de " << vNR.tamLog() << " personas las cuales recibieron vacunas fuera de su pauta recomendada." << std::endl;
        linea();
        

        
        /*Forzar la vacunación de la primera dosis de estos NSS que no han sido aún vacunados
        con una dosis de Moderna: 1622650940; 1941046560; 1756824615; 1625692780;
        1855345010.*/
        
        std::string vAux[] = {"1622650940","1941046560","1756824615","1625692780","1855345010"};
        
        for( int i = 0; i < 5; i++ ){
            
            u = gv.buscarUsuario( vAux[i]);
            //¿Está?
            if( u != 0 ){
                if( u->edad() > 12 ){
                    gv.administrarDosis( u, Dosis::Moderna );
                    std::cout << "Se ha administrado una dosis de Moderna a " << u->GetNSS() << std::endl;
                }
                else
                    std::cout << "El usuario es menor de 12 años:  --> " << vAux[i] << std::endl;
            }
            else
                std::cout << "No se encontró ningún usuario con NSS --> " << vAux[i] << std::endl;
        }
        
        std::cout << "\nComprobamos el número de vacunas de los últimos usuarios a los que les forzamos vacunación " <<
                 " con una dosis de Moderna. " << std::endl;
        for( int i = 0; i < 5; i++ ){
            u = gv.buscarUsuario( vAux[i]);
            if( u != 0 && u->edad() > 12)
                std::cout << vAux[i] << ": " << u->getNDosis() << " dosis." << std::endl;            
        }
        linea();
        
        std::cout << "Número de vacunas en almacén: " << gv.vacunasAlmacen();
        
    }catch(std::bad_alloc &ba){
        std::cerr << "Excepción bad_alloc recogida: " << ba.what() << std::endl;
    }catch(std::runtime_error &re){
        std::cerr << "Excepción runtime_error recogida: " << re.what() << std::endl;
    }catch(std::out_of_range &oor){
        std::cerr << "Excepción out_of_range recogida: " << oor.what() << std::endl;
    }
    return 0;
}