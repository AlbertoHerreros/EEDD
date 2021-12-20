
#include <vector>
#include <iostream>
#include <string>
#include <ratio>
#include "GestionVacunas.h"
#include "img.h"

#include <Windows.h>

using namespace std;

void linea(){
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

int main() {
    try{      
        GestionVacunas gv("usuarios.txt","dosis.txt","centros.txt");
        gv.estadoCarga();
        std::vector<std::string> vNSS = gv.listadoNSS();

        /* Realizar vacunación masiva con la primera dosis a todos los usuarios mayores de 12
        años menos a los que tenga NSS acabado en 5 o 7.*/
        int cont = 0;
        for( int i = 0; i < vNSS.size(); i++ ){
            
            if( vNSS[i][9] != '5' && vNSS[i][9] != '7'){
                
                Usuario *u = gv.buscarUsuario(vNSS[i]);
                std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                TarjetaVacunacion *t = 0;
                gv.buscarTarjeta(claveTarjeta,t);
                if( t != 0){
                    if( gv.vacunarUsuario(t) )
                       cont++; 
                } 
            }
        }
        linea();
        std::cout << "Se han vacunado a los usuarios acabados su NSS en 5 o 7: " << cont << " usuarios." << std::endl;

        /* Vacunar de nuevo a todos los usuarios mayores de 25 años.*/
        cont = 0;
        for( int i = 0; i < vNSS.size(); i++ ){
            
            Usuario *u = gv.buscarUsuario(vNSS[i]);
            
            if( u->edad() > 25 ){
                std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                TarjetaVacunacion *t = 0;
                gv.buscarTarjeta(claveTarjeta,t);
                if( t != 0){
                    if( gv.vacunarUsuario(t) )
                       cont++; 
                } 
            }
        }
        std::cout << "Se han vacunado a los usuarios mayores de 25: " << cont << " usuarios." << std::endl;

        /* Comprobar cuántos usuarios tienen pasaporte covid (pauta completa) hasta el
        momento con las condiciones actuales: dos dosis en mayores de 12 años.*/
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
        linea();
        std::cout << "Disponen de pasaporte covid " << cont << " usuarios." << std::endl;
        linea();

//        std::cout << "MOSTRAR PASAPORTES COVID -> PULSAR ENTER." << std::endl;
//        getchar();
//        for( int i = 0; i < hexCodes.size(); i++ )
//            std::cout << hexCodes[i] << std::endl;
        
        /* Cambiamos las nuevas condiciones de pasaporte covid: ahora deben ser todos los
        mayores de 5 años con 2 dosis y los mayores de 60 con 3 dosis y comprobamos
        cuantas personas obtendrían el pasaporte covid ahora sin nuevas vacunaciones. Hacer
        nuevas versiones de las funciones pautaCompleta() y pasaporteCovid()*/
        cont = 0;
        hexCodes.clear();
        for( int i = 0; i < vNSS.size(); i++ ){

                Usuario *u = gv.buscarUsuario(vNSS[i]);
                
                    std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                    TarjetaVacunacion *t = 0;
                    gv.buscarTarjeta(claveTarjeta,t);
                    
                    if( t != 0){
                        if( gv.pautaCompleta2(t) ){
                            hexCodes.push_back(t->pasaporteCovidCode());
                            cont++;
                        }      
                }
        }
        std::cout << "Disponen de pasaporte covid bajo la nueva normativa " << cont << " usuarios." << std::endl;
        linea();
        
        /* Hacer la llamada a la vacunación en todos los centros y para todas las tarjetas de
        usuarios de entre 5 y 11 años en un radio de 0.35 grados.*/
        std::vector<std::string> vIDTarjetas;
        std::vector<std::string> aux;
        
        vIDTarjetas = gv.llamadaVacunacionMenores( 0.35 );
        /*De todos los llamados, vacunar automáticamente a todos los que tengan al menos un 5
        en su Id de tarjeta.*/
        
        bool tiene5 = false;
        
        for( int i = 0; i < vIDTarjetas.size(); i++ ){

            for( int j = 0; j < vIDTarjetas[i].size(); j++){
                
                if( vIDTarjetas[i][j] == '5')
                    tiene5 = true;
                if( tiene5 ){
                    aux.push_back(vIDTarjetas[i]);
                    break;
                }
            }
            tiene5 = false;
        }    
//        for( int i = 0; i < aux.size(); i++)
//            std::cout << aux[i] << std::endl;
        
        cont = gv.vacunarConjuntoTarjetas(aux);
        std::cout << "Llamada de vacunación para edades [5,11] con radio 0.35: " << std::endl;
        std::cout << "\t- Se encontraron " << vIDTarjetas.size() << " usuarios." << std::endl;
        std::cout << "\t- De ellos, contenían un 5 en su Id de TarjetaVacunación y fueron vacunados --> " << cont << " usuarios." << std::endl;
        
        /* Llamad a la vacunación a los mayores de 60 en un radio de 0.5 grados de los centros
        de vacunación.*/
        vIDTarjetas = gv.llamadaVacunacionMayoresDe( 60, 0.5 );
        aux.clear();
        /*De todos los llamados, vacunar automáticamente al 90% sin importar el orden.*/
        int noventa = vIDTarjetas.size()*0.9;
        for( int i = 0; i < noventa; i++ )
            aux.push_back( vIDTarjetas[i] );
        
        cont = gv.vacunarConjuntoTarjetas(aux);
        std::cout << "\nLlamada de vacunación para edades [60,+] con radio 0.5: " << std::endl;
        std::cout << "\t- Se encontraron " << vIDTarjetas.size() << " usuarios." << std::endl;
        std::cout << "\t- De ellos, fueron vacunados --> " << cont << " usuarios." << std::endl;
        
        /*Comprobad de nuevo el número de personas con pauta completa considerando que
        relajamos la condición para los niños de entre 5 a 11 años, bastando con una sola
        dosis. Hacer por tanto nuevas versiones de las funciones asociadas*/
        cont = 0;
        hexCodes.clear();
        for( int i = 0; i < vNSS.size(); i++ ){

                Usuario *u = gv.buscarUsuario(vNSS[i]);
                
                    std::string claveTarjeta = u->GetNSS() + u->GetNombre();
                    TarjetaVacunacion *t = 0;
                    gv.buscarTarjeta(claveTarjeta,t);
                    
                    if( t != 0){
                        if( gv.pautaCompleta3(t) ){
                            hexCodes.push_back(t->pasaporteCovidCode());
                            cont++;
                        }      
                }
        }
        linea();
        std::cout << "Disponen de pasaporte covid bajo la nueva normativa " << cont << " usuarios." << std::endl;
        linea();
        
    /*VOLUNTARIO: Visualización 2D con la clase Img*/ 
    RGBColor blanco (255,255,255);
    
    Img img(600, 600, blanco);
    int nfilas = img.numFilas();
    int ncol = img.numColumnas();
    
    cout << "Imagen creada con " << nfilas << "filas y " << ncol << " columnas." <<endl;
    
    int r = 0; // azul
    int g = 0;
    int b = 255;
    // sabemos el tamaño de la caja envolvente de los datos, pero volver a calcular 
    double minlat = 35.86688;
    double maxlat = 43.272616;
    double minlon = -9.99443;
    double maxlon = 3.98926;
    // Calculamos el número de pixeles asociados a un grado 

    cout << "lat: xmin = " << minlat <<  ", " << "xmax = " << maxlat << endl;
    cout << "lon: ymin = " << minlon <<  ", " << "ymax = " << maxlon << endl;

    double rangox = maxlat - minlat;
    double rangoy = maxlon - minlon;

    cout << "rango x = " << rangox << endl;
    cout << "rango y = " << rangoy << endl;

    cout << "nfilas = " << nfilas << endl;
    cout << "ncol = " << ncol << endl;

    double pixelPorGradoX = (double) (nfilas - 1) / rangox;
    double pixelPorGradoY = (double) (ncol - 1) / rangoy;

    cout << "Pixel por Grado X = " << pixelPorGradoX << endl;
    cout << "Pixel por Grado Y = " << pixelPorGradoY << endl;

    // elegimos las coordenada de un recuadro cualquiera
    double lat1 = 40;
    double lon1 = -8;

    double lat2 = 42.0;
    double lon2 = -5.0;

    img.pintarRecuadro((lat1 - minlat) * pixelPorGradoX, ncol - 1 - ((lon1 - minlon) * pixelPorGradoY),
                            (lat2 - minlat) * pixelPorGradoX, ncol - 1 - ((lon2 - minlon) * pixelPorGradoY), 255, 0, 0);

    // ejemplo de punto que debe estar por el centro, lo pintamos acto seguido
    double vlat = 39.569748;
    double vlon = -3.002585;

    int posX = (vlat - minlat) * pixelPorGradoX;
    int posY = ncol - 1 - ((vlon -minlon) * pixelPorGradoY);
    img.pintarPixelGrande(posX,posY,r,g,b);
    //img.pintarPixel(posX,posY,r,g,b);
  
    //PINTAR TODOS LOS USUARIOS SEGÚN SU CENTRO CERCANO
    for( int i = 0; i < vNSS.size(); i++ ){
            
        Usuario *u = gv.buscarUsuario(vNSS[i]);
            
        if( u->edad() > 25 ){
            std::string claveTarjeta = u->GetNSS() + u->GetNombre();
            TarjetaVacunacion *t = 0;
            gv.buscarTarjeta(claveTarjeta,t);
            if( t != 0){
                
                posX = (t->getDomicilio().lat - minlat) * pixelPorGradoX;
                posY = ncol - 1 - ((t->getDomicilio().lon - minlon) * pixelPorGradoY);
                
                switch( t->getIdCentroCercano() ){
                    case 1:
                        img.pintarPixel(posX, posY, 0,255,32); //Verde fosforito
                    break;
                    
                    case 2:
                        img.pintarPixel(posX, posY, 255,0,229); //Rosa
                    break;
                    
                    case 3:
                        img.pintarPixel(posX, posY, 0,32,255); //Azul oscuro
                    break;
                    
                    case 4:
                        img.pintarPixel(posX, posY, 48,255,255); //Azul celeste
                    break;
                    
                    case 5:
                        img.pintarPixel(posX, posY, 0,0,0); //Negro
                    break;
                }    
            } 
        }
    }
    
    try {
        linea();
        std::cout << "PULSA ENTER PARA VISUALIZAR LA IMAGEN" << std::endl;
        getchar();
        img.guardar("./mapaUsaResult.ppm");
        ShellExecuteW(NULL, L"open", L"C:\\Users\\Alberto Limas\\Documents\\NetBeansProjects\\EEDD_2021_2022\\pr6\\mapaUsaResult.ppm", L"", L"C:\\path\\to\\my\\", SW_SHOW);
        //system("explorer C:\\Users\\Alberto Limas\\Documents\NetBeansProjects\EEDD_2021_2022\pr6\mapaUsaResult.ppm ");
    }
    catch(ErrorEscrituraFichero &e) {
    	cout << "Error al escribir fichero" << endl;
    	return 1;
    }
    
    cout << "Operación realizada con exito, ahora visualizarlo con cualquier visor de imágenes" << endl;

    }catch(std::bad_alloc &ba){
        std::cerr << "Excepción bad_alloc recogida: " << ba.what() << std::endl;
    }catch(std::runtime_error &re){
        std::cerr << "Excepción runtime_error recogida: " << re.what() << std::endl;
    }catch(std::out_of_range &oor){
        std::cerr << "Excepción out_of_range recogida: " << oor.what() << std::endl;
    }
    return 0;
}