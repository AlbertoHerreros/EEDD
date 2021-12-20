
#include <vector>
#include <iostream>
#include <string>
#include "GestionVacunas.h"

using namespace std;

void linea(){
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

int main() {
    try{
        linea();
        GestionVacunas gv("usuarios.txt","dosis.txt","centros.txt");
        gv.estadoCarga();
        linea();
        
        /* Mostrar el número de dosis total almacenadas de cada tipo usando el método
        GestionVacunas::numTotalVacunasTipo().*/
        std::cout << "Total de dosis en cada centro según su fabricante: ";
        std::cout << "\n --> Pfizer: \n" << gv.numTotalVacunasTipo(Dosis::Pfizer) << " dosis." << std::endl;
        std::cout << "\n --> AstraZeneca: \n" << gv.numTotalVacunasTipo(Dosis::AstraZeneca) << " dosis." << std::endl;
        std::cout << "\n --> Moderna: \n" << gv.numTotalVacunasTipo(Dosis::Moderna) << " dosis." << std::endl;
        std::cout << "\n --> Johnson: \n" << gv.numTotalVacunasTipo(Dosis::Johnson) << " dosis." << std::endl;
        linea();
        
        /**Vacunar a todos los usuarios cuyo NSS sea par empleando para ello el vecAuxiliar de
        la práctica anterior y buscar dichas claves en el Map<String, Usuario>.*/
        std::vector<std::string> vNSS = gv.listadoNSS();
        int total = 0;
        
        for( int i = 0; i < vNSS.size(); i++ ){
            //Último caracter NSS lo pasamos a int
            char c = vNSS[i].back();
            int n = (int)c;
            //Si par se vacuna
            if( n % 2 == 0 ){
                //Buscamos Usuario --> Obtenemos centro en el que se vacuna --> Intentamos vacunarlo 
                Usuario *u = gv.buscarUsuario(vNSS[i]);
                if( u->edad() > 12){   
                    CentroVacunacion *cv = gv.vacunarUsuario(u);
                    if ( cv )
                       total++;   
                    }
            }
        }
        std::cout << "Vacunación de todos los Usuarios con NSS par: \n\t- Se han vacunado " << total << " usuarios." << std::endl;
        
        /*Vacunar a todos los usuarios con más de 65 años.*/
        total = 0;
        
        for( int i = 0; i < vNSS.size(); i++ ){
            Usuario *u = gv.buscarUsuario(vNSS[i]);
            //Si edad > 65 se vacuna
            if( u->edad() > 65 ){
                //Obtenemos centro en el que se vacuna --> Intentamos vacunarlo 
                CentroVacunacion *cv = gv.vacunarUsuario(u);
                if ( cv )
                    total++;       
            }
        }
        std::cout << "Vacunación de todos los Usuarios mayores de 65 años: \n\t- Se han vacunado " << total << " usuarios." << std::endl;
        linea();
        /*Vacunar a todas las usuarias que se llaman Eva, mostrar su nombre completo y el id y la dirección del centro donde han sido vacunadas.*/
        total = 0;
        
        for( int i = 0; i < vNSS.size(); i++ ){
            Usuario *u = gv.buscarUsuario(vNSS[i]);
            //Si se llama Eva se vacuna
            if( u->GetNombre() == "Eva" && u->edad() > 12 ){
                //Obtenemos centro en el que se vacuna --> Intentamos vacunarlo 
                CentroVacunacion *cv = gv.vacunarUsuario(u);
                if( cv ){
                    std::cout << u->GetNombre() << " " << u->GetApellidos() << " " << " <-> Vacunada en el centro <-> " << cv->getId() 
                            << " <-> " << cv->getLatitud() << " " << cv->getLongitud() << std::endl;
                    total++;
                }
            }
        }
        std::cout << "\nVacunación de todos los Usuarios cuyo nombre es Eva: \n\t- Se han vacunado " << total << " usuarios." << std::endl;
        linea();
        /*Si en algún momento del proceso de vacunación un centro se queda sin dosis en
        almacén, leer del fichero las primeras 100 dosis sobrantes (a partir de la última dosis
        leída del fichero), añadirlas al multimap y proseguir con la vacunación.*/
        
        /*De igual forma que en la práctica anterior, mientras haya dosis de las recomendadas
        se ponen éstas, si no se ponen de cualquier otro fabricante. Siempre se da prioridad
        a las dosis que ya están en el CentroVacunación, por lo que si en un centro no
        quedan dosis de un tipo determinado, se vacunará con una dosis de otro tipo,
        independientemente de si hay del tipo recomendado en las dosis no leídas del fichero.*/
        
        /*  Después de la vacunación, mostrar de nuevo el número total de dosis almacenadas de
        cada tipo.*/
        std::cout << "Total de dosis en cada centro según su fabricante: ";
        std::cout << "\n --> Pfizer: \n" << gv.numTotalVacunasTipo(Dosis::Pfizer) << " dosis." << std::endl;
        std::cout << "\n --> AstraZeneca: \n" << gv.numTotalVacunasTipo(Dosis::AstraZeneca) << " dosis." << std::endl;
        std::cout << "\n --> Moderna: \n" << gv.numTotalVacunasTipo(Dosis::Moderna) << " dosis." << std::endl;
        std::cout << "\n --> Johnson: \n" << gv.numTotalVacunasTipo(Dosis::Johnson) << " dosis." << std::endl;
        linea();
        
    }catch(std::bad_alloc &ba){
        std::cerr << "Excepción bad_alloc recogida: " << ba.what() << std::endl;
    }catch(std::runtime_error &re){
        std::cerr << "Excepción runtime_error recogida: " << re.what() << std::endl;
    }catch(std::out_of_range &oor){
        std::cerr << "Excepción out_of_range recogida: " << oor.what() << std::endl;
    }
    return 0;
}