/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VDinamico.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 27 de septiembre de 2021, 17:19
 */

#ifndef VDINAMICO_H
#define VDINAMICO_H

#include <algorithm>
#include <climits>
#include <stdexcept>
#include <iostream>

template<class T>
class VDinamico{
    private:
        
        int _taml, _tamf;   //Tamaños lógico y físico del VDinámico respectivamente
        T* _mem;            //VDinámico de elementos del tipo <T>
        
        int sigPotencia2( int n );
        
    public:
        
        VDinamico( unsigned int tamLog = 0 ); 
        VDinamico( const VDinamico<T>& orig );
        VDinamico( const VDinamico<T>& orig, unsigned int posicionInicial, unsigned int numElementos );
        
        VDinamico<T>& operator=( const VDinamico<T> &orig );
        T& operator[]( unsigned pos ){ return _mem[pos]; }
        
        void insertar( const T&dato, unsigned int pos = UINT_MAX );
        T borrar( unsigned int pos = UINT_MAX );
        
        void ordenar(){ std::sort(_mem, _mem+_taml);}
        void ordenarRev(){
            std::sort( _mem,_mem+_taml );
            std::reverse( _mem,_mem+_taml);}
        
        int busquedaBin( T& dato );
        
        unsigned int tamLog(){ return _taml; } /*Devuelve el tamaño lógico del VDinámico*/
        ~VDinamico(){ delete []_mem; } /*Destructor de la clase VDinámico*/
};

/**
 * @brief Función que devuelve la potencia de 2 más cercana por arriba de un número (mayor o igual)
 * @param n: número del cual queremos obtener 2^x >= n
 * @return potencia de 2 >= n
 */
template<class T>
VDinamico<T>::sigPotencia2(int n){
    int c = 1;
    while( c < n )
        c *= 2;
    return c;
}
/**
 * @brief Constructor parametrizado, _taml = 0, _tamf = primera potencia 2^x que sea mayor que
 *        tamLog, puede actuar como constructor por defecto con _taml = 0 y _tamf = 1
 * @param tamLog: número que hay que igualar o superar con una potencia de 2
 */
template<class T>
VDinamico<T>::VDinamico(unsigned int tamLog){
    _mem = new T[ _tamf = sigPotencia2(tamLog)];
    _taml = 0;
}
/**
 * @brief Constructor copia del VDinámico
 * @param orig: VDinámico que queremos copiar
 */
template<class T>
VDinamico<T>::VDinamico(const VDinamico<T>& orig):
_tamf(orig._tamf),_taml(orig._taml){
    
    _mem = new T[ _tamf ];
    for( int i = 0; i < _taml; i++ )
        _mem[i] = orig._mem[i];
}
/**
 * @brief Constructor copia parcial del VDinámico
 * @param orig: VDinámico que queremos copiar parcialmente
 * @param posicionInicial: posición desde la que empezamos a copiar
 * @param numElementos: n elementos que copiamos desde la posición inicial
 */
template<class T>
VDinamico<T>::VDinamico(const VDinamico<T>& orig, unsigned int posicionInicial, unsigned int numElementos){ 
    
    _mem = new T[ _tamf = sigPotencia2(numElementos) ];
    
    int pos = 0; //Posición inicial del vector _mem, ¡la posición i no comienza en 0!
    for( int i = posicionInicial; i == posicionInicial+numElementos; i++ )
        _mem[pos++] = orig[i]; 
    _taml = numElementos; // _taml de _mem será el numElementos que hemos copiado
}
/**
 * @brief Operador de asignación del VDinámico
 * @param orig: VDinámico que queremos copiar
 * @return referencia a VDinámico
 */
template<class T>
VDinamico<T>& VDinamico<T>::operator=( const VDinamico<T>& orig){
    delete []_mem;
    _mem = new T[ _tamf = orig._tamf ];
    
    for( int i = 0; i < orig._taml; i++ )
        _mem[i] = orig._mem[i];
    _taml = orig._taml;
    return *this;
}
/**
 * @brief Función para insertar datos en el VDinámico en una posición, si no se determina por el final
 * @param dato: dato a insertar en el vector
 * @param pos: posición de inserción, no tiene por qué estar determinada
 */
template<class T>
void VDinamico<T>::insertar(const T& dato, unsigned int pos){
    //Hacer crecer el vector si se cumple _taml == _tamf
    if( _taml == _tamf ){
        T *vaux = new T[ _tamf = _tamf*2 ];
        for( int i = 0; i < _taml; i++ )
            vaux[i] = _mem[i];
        delete []_mem;
        _mem = vaux;
    }
    //Inserción al final
    if( pos == UINT_MAX ) 
        _mem[_taml++] = dato; 
    else{
        //Posición fuera de rango
        if( pos > _tamf)
            throw std::out_of_range("Inserción en la posición " + std::to_string(pos) + " fuera de rango.");
        //Inserción por la mitad
        for( unsigned i = _taml-1; i >= pos; i--)
            _mem[i+1] = _mem[i];
        _mem[pos] = dato; 
        _taml++;
    }
}
/**
 * @brief Función para borrar datos del VDinámico, si no se determina la posición se borrará por el final
 * @param pos: posición en la que se efectúa el borrado
 * @return dato borrado
 */
template<class T>
T VDinamico<T>::borrar(unsigned int pos){
    //Disminuir vector si se cumple _taml*3<_tamf
    if( _taml*3<_tamf ){
        _tamf = _tamf/2;
        T *vaux = new T[_tamf];
        for( unsigned i = 0; i < _taml; i++ )
            vaux[i] = _mem[i];
        delete []_mem;
        _mem = vaux;
    }
    //Borrado por el final
    if( pos == UINT_MAX ) 
        return _mem[--_taml]; 
    else{
        //Posición fuera de rango
        if( pos > _tamf)
            throw std::out_of_range("Borrado en la posición " + std::to_string(pos) + " fuera de rango.");
        //Borrado por la mitad
        for( unsigned i = pos; i < _taml; i++ )
            _mem[i] = _mem[i+1];
            return _mem[--_taml];
    }
}
/**
 * @brief Búsqueda binaria en el VDinámico
 * @param dato: dato a buscar en el VDinámico
 * @return posición del dato si ha sido encontrado, sino devuelve -1
 */
template<class T>
int VDinamico<T>::busquedaBin(T& dato){
    int inf = 0;
    int sup = _taml-1;
    int mid;
    
    while( inf <= sup ){
        mid = (inf + sup)/2;
        if( _mem[mid] == dato )
            return mid;
        else if( _mem[mid] < dato ) 
            inf = mid+1;
        else 
            sup = mid - 1;
    }
    return -1;
}
#endif /* VDINAMICO_H */
