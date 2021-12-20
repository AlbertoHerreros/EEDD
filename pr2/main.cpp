#include <iostream>
#include <fstream>
#include <string>
#include "VDinamico.h"
#include "Dosis.h"
#include "Usuario.h"
#include "ListaEnlazada.h"

using namespace std;

void linea(){
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

int main() {
    try{ 
        //Instanciamos la ListaEnlazada de usuarios usando usuarios.txt
        ListaEnlazada<Usuario> lUsuarios;
        
        ifstream iss("usuarios.txt");
	string palabra;
	int corte = 0;

	std::string nombre;
	std::string apellidos;
	std::string NSS;
	int dia = 0;
	int mes = 0;
	int anno = 0;
//      int lat;
//      int lon;
        
	clock_t t_ini = clock();

	while (getline(iss, palabra)) {

                corte = palabra.find(';');
		nombre = (palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find(';');
		apellidos = (palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find(';');
		NSS = (palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find('/');
		dia = stoi(palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		corte = palabra.find('/');
		mes = stoi(palabra.substr(0, corte));
		palabra.erase(0, corte + 1);

		anno = stoi(palabra);
                
                Fecha fNac = Fecha(dia,mes,anno);
                Usuario u = Usuario(nombre, apellidos, NSS, fNac);
                lUsuarios.insertaFinal(u);     
	}
	iss.close();
	cout << "Tiempo de carga de los " << lUsuarios.tam() << " usuarios: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        //Instanciamos el vector dinámico de dosis
        VDinamico<Dosis> vDosis;
        //Abrimos el fichero y creamos las variables para la lectura de los datos de dosis.txt
        ifstream is("dosis.txt");

	int id = 0;
	int idLote = 0;
	int fabricante = 0;
        
	t_ini = clock();

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
	std::cout << "Tiempo de carga de las 10000 dosis: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        
        //Listar por pantalla las dosis administradas a los 50 primeros usuarios de la lista
        vDosis.ordenar(); //Ordenar por fecha de fabricación para suministrar las dosis
        
        ListaEnlazada<Usuario>::Iterador<Usuario> it = lUsuarios.iterador();
        t_ini = clock();
        int cont = 0;
        Dosis* dd;
        while( !it.fin() ){
            dd = &vDosis[cont++];
            it.dato().administrarVacuna(dd);
            it.siguiente();
        }
        it = lUsuarios.iterador();

        std::cout << "Tiempo de administración de las 10000 vacunas: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        linea();
        std::cout << "Primeras 50 dosis administradas: " << std::endl;
        it = lUsuarios.iterador();
        cont = 0;
        while( !it.fin() && cont < 50 ){
            cout << cont+1;
            it.dato().mostrarDosis();
            it.siguiente();
            cont++;
        }
        linea();
        
        //Mostrar por pantalla la información de los usuarios con NSS 1491983009, 1280280451, 1696453083, incluyendo su edad y la dosis administrada
        std::cout << "Mostrar datos de Usuarios con NSS 1491983009, 1280280451, 1696453083" << std::endl;
        
        it = lUsuarios.iterador();
        while( !it.fin() ){
            
            if( it.dato().GetNSS() == "1491983009" || it.dato().GetNSS() == "1280280451" || it.dato().GetNSS() == "1696453083" ){
                it.dato().mostrarUsuario();
                it.dato().mostrarDosis();
            }
            it.siguiente();
        }
//        Mostrar todos los usuarios
//        cont = 0;
//        it = lUsuarios.iterador();
//        while( !it.fin() ){
//            std::cout << "USUARIO " << cont++ << ": ";
//            it.dato().mostrarUsuario();
//            it.siguiente();
//        }
        
        /* Opcional: Buscar cual es el nombre que más se repite en la lista de
         * usuarios y construir una nueva lista con dichos usuarios. Mostrar la nueva lista y su tamaño.
         * Este apartado debe hacerse antes del apartado 6 (borrar los usuarios con nombre Joan).*/
         
        //En ambos vectores, la misma posición [i] determina el nombre de aquellos Usuarios (vNombres) que comparten nombre y 
        //el número de ocurrencias (vContadorNombres)
        VDinamico<std::string> vNombres;
        VDinamico<int> vContadorNombres;
        bool encontrado;
        it = lUsuarios.iterador();
        std::string nombreBusqueda;
        int posicion;
        int max = 0;
        int posMax = 0;
        
        linea();
        std::cout << "Buscar el usuario que tiene mayor número de ocurrencias: " << std::endl;
        
        t_ini = clock();
        
        while( !it.fin() ){
            
            encontrado = false;
            nombreBusqueda = it.dato().GetNombre();
            
            for( int i = 0; i < vNombres.tamLog(); i++ ){
                if( vNombres[i] == nombreBusqueda ){
                    encontrado = true;
                    posicion = i;
                }
            }
            if( encontrado ){
                vContadorNombres[posicion] = vContadorNombres[posicion] + 1;
                //Determinar el nombre con mayor número de repeticiones -> Guardamos su posición
                if( vContadorNombres[posicion] > max ){
                    max = vContadorNombres[posicion];
                    posMax = posicion;
                }
            }
            else{
                vNombres.insertar(nombreBusqueda);
                vContadorNombres.insertar(1);
            }
            it.siguiente();
        }
        std::cout << "Tiempo de búsqueda del usuario con más ocurrencias: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        std::cout << "El usuario con más ocurrencias es " << vNombres[posMax] << " con un total de " << vContadorNombres[posMax] << " apariciones." << std::endl;
        
//        //COMPROBACIÓN DEL TOTAL DE USUARIOS
//        int total = 0;
//        for( int i = 0; i < vNombres.tamLog(); i++ ){
//            std::cout << i+1 << " --> " << vNombres[i] << " <-> " << vContadorNombres[i] << " ocurrencias." << std::endl;
//            total += vContadorNombres[i];
//        }
//        std::cout << "Total de Usuarios:  " << total << std::endl;
        
        /* Buscar de dicha lista aquellos usuarios que tienen como nombre Joan, indicar cuántos hay y
         * borrarlos de la lista. Volver a buscar Joan en la lista de usuarios para comprobar que se han
         * borrado correctamente.*/
        
        cont = 0;
        Usuario uJoan = Usuario( "Joan","","",Fecha() );
                 
        it = lUsuarios.iterador();
        t_ini = clock();
        while( !it.fin() ){
            
            if( it.dato().GetNombre() == "Joan" ){
                lUsuarios.borra(it);
                cont++;
            }
            it.siguiente();
        }
        
//        MÉTODO INEFICIENTE -> Por cada usuario llamado Joan recorremos la lista una vez ( O(n) por Usuario )
//        mientras que si la iteramos solo la recorremos una vez O(n)
//        
//        ListaEnlazada<Usuario>::Iterador<Usuario> itJoan;
//        t_ini = clock();
//        do{
//            encontrado = lUsuarios.buscar(uJoan,itJoan);
//            if( encontrado ){
//                lUsuarios.borra(itJoan);
//                cont++;
//            }
//        }while( encontrado == true );

        linea();
        std::cout << "Tiempo de borrado de los " << cont << " Usuarios llamados Joan: " << ((clock() - t_ini) / (float)CLOCKS_PER_SEC) << " segs." << endl;
        //Mostrar el tamaño de la lista de usuarios una vez realizado el apartado anterior( borrado de aquellos usuarios llamados Joan)
        std::cout << "Tamaño total de la lista de usuarios tras el borrado: " << lUsuarios.tam() << std::endl;
        
        std::cout << "\t --> Comprobamos el estado de los usuarios llamados Joan: " << std::endl;
        encontrado = lUsuarios.buscar(uJoan,it);
        if( encontrado )
            std::cout << "\t\t + Aún quedan usuarios llamados Joan en la lista." << std::endl;
        else
            std::cout << "\t\t - No existen usuarios llamados Joan en la lista." << std::endl;
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