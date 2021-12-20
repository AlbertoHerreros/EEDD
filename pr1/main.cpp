#include <iostream>
#include <fstream>
#include <string>
#include "VDinamico.h"
#include "Dosis.h"

using namespace std;

void linea(){
    std::cout << "-------------------------------------------------" << std::endl;
}

int main() {
    try{ 
        //Instanciamos el vector dinámico de dosis
        VDinamico<Dosis> vDosis;
        //Abrimos el fichero y creamos las variables para la lectura de los datos de dosis.txt
        ifstream is("dosis.txt");
	string palabra;
	int corte = 0;

	int id = 0;
	int idLote = 0;
	int fabricante = 0;
	int dia = 0;
	int mes = 0;
	int anno = 0;
        
	clock_t t_ini = clock();

	while (getline(is, palabra)) {

		corte = palabra.find(';');
		id = stoi(palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find(';');
		idLote = stoi(palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find(';');
		fabricante = stoi(palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find('/');
		dia = stoi(palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find('/');
		mes = stoi(palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		anno = stoi(palabra);

                //Convertimos int a tipo fabricante ( enum )
                Dosis::fabricante enumFabricante;
                
                switch(fabricante){
                    case 0: enumFabricante = Dosis::Pfizer;
                    break;
                    
                    case 1: enumFabricante = Dosis::Moderna;
                    break;
                    
                    case 2: enumFabricante = Dosis::AstraZeneca;
                    break;
                    
                    case 3: enumFabricante = Dosis::Johnson;
                    break;
                }
                //Creamos una dosis y la insertamos en el vector dinámico de dosis
                Fecha ff = Fecha(dia,mes,anno);
                Fecha fc;
                fc = ff;
                fc.anadirMeses(2); 
                Dosis d = Dosis(id,idLote,enumFabricante,ff,fc);
                vDosis.insertar(d);    
	}

	is.close();

	cout << "Tiempo de carga de las 10.000 dosis: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
//        MOSTRAR TODO      
//        for( int i = 0; i < vDosis.tamLog(); i++ ){
//            std::cout << i+1 << " --> "; //¡¡Empezamos en 0!!
//            vDosis[i].mostrarDosis();
//        }
        
        //Ordenar el vector al revés < a > y mostrar 50 primeras dosis
        linea();
        std::cout << "Ordenamos el vector de menos a mayor" << std::endl;
        linea();
        
        t_ini = clock();
        vDosis.ordenar();
        std::cout << "Tiempo para ordenar el vector de menor a mayor: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << std::endl << std::endl;
        for( int i = 0; i < 50; i++ ){
            std::cout << i+1 << " --> "; //¡¡Empezamos en 0!!
            vDosis[i].mostrarDosis();
        }  
        
        //Ordenar el vector al revés > a < y mostrar 50 primeras dosis
        linea();
        std::cout << "Ordenamos el vector de mayor a menor" << std::endl;
        linea();
        
        t_ini = clock();
        std::cout << "Tiempo para ordenar el vector de mayor a menor: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << std::endl << std::endl;
        vDosis.ordenarRev();
        for( int i = 0; i < 50; i++ ){
            std::cout << i+1 << " --> "; //¡¡Empezamos en 0!!
            vDosis[i].mostrarDosis();
        }
        
        //Buscar 5 dosis distintas y mostrar su posición en el vector
        linea();
        std::cout << "Buscamos 5 dosis distintas: [346335905,999930245,165837,48615569,61385551]" << std::endl;
        linea();
        
        int numDosis[]={346335905,999930245,165837,48615569,61385551};
        int posicion;
        Dosis d; 
            
        vDosis.ordenar(); //Hay que ordenar de nuevo el vector de menor a mayor para que funcione la búsqueda binaria
        t_ini = clock();
        for( int i = 0; i < 5; i++){
            d = Dosis();
            d.SetId(numDosis[i]);
            posicion = vDosis.busquedaBin( d );
            if( posicion >= 0 )
                std::cout << "Dosis nº " << numDosis[i] << " encontrada en la posicion " << posicion << std::endl;
            else
                std::cout << "Dosis nº " << numDosis[i] << " no encontrada" << std::endl;
        }
        std::cout << "Tiempo para buscar las dosis: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << std::endl;
        
        //Borrar dosis defectuosas -> id [10000,2500000], id de lote múltiplo de 2 o 5 y fabricadas en 2020
        linea();
        std::cout << "Buscamos las dosis defectuosas" << std::endl;
        linea();
        t_ini = clock();
        
        VDinamico<Dosis> vDosisDefectuosas;
        for( int i = 0; i < vDosis.tamLog(); i++ ){
            
            if( vDosis[i].GetId() >= 10000 && vDosis[i].GetId() <= 2500000 )
                
                if( vDosis[i].GetIdLote() % 2 == 0 || vDosis[i].GetIdLote() % 5 == 0 )
                    
                    if( vDosis[i].GetFechaFabricacion().verAnio() == 2020 ){
                        vDosisDefectuosas.insertar( vDosis[i] );
                        vDosis.borrar(i);
                        i--; //Si borras una dosis y la siguiente es defectuosa también te la saltas puesto que se desplazan todas i = i-1
                    }              
        }
        
        std::cout << "Tamaño lógico del vector de Dosis: " << vDosis.tamLog() << std::endl;
        std::cout << "Tamaño lógico del vector de Dosis Defectuosas: " << vDosisDefectuosas.tamLog() << std::endl << std::endl;
        
        for( int i = 0; i < 5; i++ )
            vDosisDefectuosas[i].mostrarDosis();
       
        std::cout << "\nTiempo para buscar las dosis defectuosas: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << std::endl;
        
    }catch(std::bad_alloc &ba){
        std::cerr << "Excepción bad_alloc recogida: " << ba.what() << std::endl;
    }catch(std::runtime_error &re){
        std::cerr << "Excepción runtime_error recogida: " << re.what() << std::endl;
    }catch(std::out_of_range &oor){
        std::cerr << "Excepción out_of_range recogida: " << oor.what() << std::endl;
    }
}