/* 
 * File:   AVL.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 21 de octubre de 2021, 13:05
 */

#ifndef AVL_H
#define AVL_H

#include "Nodo.h"
#include "VDinamico.h"
#include <iostream>
#include <ostream>

template <class T>
class AVL{
    
    public:
        
        //Definición de la clase Nodo para AVL
        template <class U>
        class Nodo{
            public:
                Nodo<U> *izq;
                Nodo<U> *der;
                int bal;
                U dato;

                Nodo():izq(0),der(0),bal(0){}
                Nodo(U &ele):izq(0),der(0),bal(0),dato(ele){}
        };
        
        AVL();
        AVL<T>( const AVL<T> &orig );
        AVL<T>& operator=( const AVL<T> &orig );
        ~AVL();
        
        bool inserta( T& dato );
        T* buscaNR( T& dato, T& result );
        T* buscaIt( T& dato, T& result );
        VDinamico<T*> recorreInorden();
        
        int numElementos(){ return _nElementos; }
        unsigned int altura();
        
        friend std::ostream& operator<<( std::ostream& os, const T& obj ){
            return os << obj;
        }   
        
    private:
        Nodo<T> *_raiz;
        int _nElementos;  
        //Funciones privadas de algunos métodos públicos
        void copiaAVL( Nodo<T>* &p, Nodo<T> *orig );
        void borradoCompleto( Nodo<T> *p );
        int inserta( Nodo<T>* &c, T &dato );
        void rotDer( Nodo<T>* &nodo );
        void rotIzq( Nodo<T>* &nodo );
        
        T* busquedaRecursiva( T& dato, Nodo<T> *p );
        
        void inorden( Nodo<T> *p, int nivel, VDinamico<T*> &vaux );
        void altura( Nodo<T> *p, int nivel, int *max );
};

/**
 * @brief Constructor por defecto de AVL
 */
template<class T>
AVL<T>::AVL(){
    _raiz = 0;
    _nElementos = 0;
}

/**
 * @brief Constructor copia de AVL
 * @param orig: AVL a copiar
 */
template<class T>
AVL<T>::AVL(const AVL<T>& orig){
    copiaAVL( _raiz, orig._raiz );
    _nElementos = orig._nElementos;
}
/**
 * @brief Operador de asignación de AVL
 * @param orig: AVL a copiar
 * @return referencia a sí mismo
 */
template<class T>
AVL<T>& AVL<T>::operator =(const AVL<T>& orig){
    if( this != orig ){
        copiaAVL( _raiz, orig._raiz );
        _nElementos = orig._nElementos;
    }  
    return *this;
}
/**
 * @brief Método privado para realizar la copia en el constructor copia y operador de asignación mediante PREORDEN
 * @param p: raíz del árbol original
 * @param orig: raíz del árbol a partir de la cual copiaremos el árbol orig en el árbol original
 */
template<class T>
void AVL<T>::copiaAVL(Nodo<T>*& p, Nodo<T>* orig){
    //Recorrido en preorden
    if( orig ){
        p = new Nodo<T>( orig->dato );
        p->bal = orig->bal;
        if ( orig->izq ) copiaAVL ( p->izq, orig->izq );
        if ( orig->der ) copiaAVL ( p->der, orig->der );
    }  
}

/**
 * @brief Destructor de AVL
 */
template<class T>
AVL<T>::~AVL(){
    if( _raiz ) borradoCompleto( _raiz );
}
/**
 * @brief Método privado para llevar a cabo la destrucción de un AVL usando un recorrido POSTORDEN
 * @param p: cada uno de los nodos a recorrer
 */
template<class T>
void AVL<T>::borradoCompleto(Nodo<T>* p){
    if( p ){
        borradoCompleto( p->izq );
        borradoCompleto( p->der );
        delete p;
    }
}

/**
 * @brief Método público para insertar un dato en el AVL
 * @param dato: dato a insertar
 * @return true si se pudo instanciar, false en caso contrario
 */
template<class T>
bool AVL<T>::inserta( T& dato ){ 
    return inserta( _raiz, dato );
}
/**
 * @brief Método privado para la inserción de un dato en el AVL
 * @param c: nodo raíz del AVL al que insertamos un dato
 * @param dato: dato a insertar
 * @return deltaH: 
 */
template<class T>
int AVL<T>::inserta( Nodo<T>* &c, T &dato ){
    Nodo<T> *p = c;
    int deltaH = 0;
    if( !p ){
        p = new Nodo<T>(dato);
        c = p;
        deltaH = 1;
        _nElementos++;
    }
    else if( dato > p->dato ){
        if( inserta( p->der, dato ) ){
            p->bal--;
            if( p->bal == -1 )
                deltaH = 1;
            else if( p->bal == -2 ){
                if( p->der->bal == 1 ) rotDer(p->der);
                rotIzq(c);
            } } }
    else if( dato < p->dato ){
        if( inserta( p->izq, dato ) ){
            p->bal++;
            if( p->bal == 1 ) 
                deltaH = 1;
            else if( p->bal == 2 ){
                if( p->izq->bal == -1 ) rotIzq(p->izq);
                rotDer(c);
            } } }
    return deltaH;
}
/**
 * @brief Método para realizar rotaciones a la derecha en la inserción
 * @param p: nodo
 */
template<class T>
void AVL<T>::rotDer(Nodo<T>* &p){
    Nodo<T> *q = p, *l;
    p = l = q->izq;
    q->izq = l->der;
    l->der = q;
    q->bal--;
    if( l->bal > 0 ) q->bal -= l->bal;
    l->bal--;
    if( q->bal < 0 ) l->bal -= -q->bal;
}
/**
 * @brief Método para realizar rotaciones a la izquierda en la inserción
 * @param p: nodo
 */
template<class T>
void AVL<T>::rotIzq(Nodo<T>* &p){
    Nodo<T> *q = p, *r;
    p = r = q->der;
    q->der = r->izq;
    r->izq = q;
    q->bal++;
    if( r->bal < 0 ) q->bal += -r->bal;
    r->bal++;
    if( q->bal > 0 ) r->bal += q->bal;
}

/**
 * @brief Método público para realizar la búsqueda recursiva en el AVL
 * @param dato: dato a buscar
 * @param result: objeto encontrado
 * @return true si se encontró, false en caso contrario
 */
template<class T>
T* AVL<T>::buscaNR(T& dato, T& result){
    
    T *p = busquedaRecursiva(dato,_raiz);
    
    if( p != 0){
        result = *p;
        return p;
    }
    result = T();
    return 0;    
}
/**
 * @brief Método privado para la búsqueda recursiva en el AVL
 * @param dato: dato a buscar
 * @param p: nodo a recorrer ( raíz )
 * @return Nodo con el dato encontrado o nullptr si no está
 */
template<class T>
T* AVL<T>::busquedaRecursiva(T& dato, Nodo<T>* p){
    if( p ){
        if( dato == p->dato ){
            return &(p->dato);
        }
        else if( dato > p->dato )
            busquedaRecursiva( dato, p->der );
        else
            busquedaRecursiva( dato, p->izq );  
    }
    else
        return 0;
}
/**
 * @brief Método para buscar de manera iterada un dato en el AVL
 * @param dato: dato a buscar
 * @param result: objeto que contiene el dato encontrado
 * @return true si se encontró, false en caso contrario
 */
template<class T>
T* AVL<T>::buscaIt(T& dato, T& result){
    Nodo<T> *p = _raiz;
    while( p != 0 ){
        if( dato == p->dato ){
            result = p->dato;
            return &(p->dato);
            }
        else if( dato > p->dato )
            p = p->der;
        else
            p = p->izq;
    }
    return false;    
}

/**
 * @brief Método público para mostrar el recorrido en INORDEN del AVL
 * @return VDinamico<T*> con los datos ordenados según el recorrido del AVL en INORDEN 
 */
template<class T>
VDinamico<T*> AVL<T>::recorreInorden(){
    VDinamico<T*> vaux;
    inorden( _raiz,0,vaux );
    return vaux;
}
/**
 * @brief Método privado para mostrar el recorrido en INORDEN del AVL e introducir los datos en un VDinamico
 * @param p: nodo raíz del que partiremos
 * @param nivel: nivel en el que se encuentra cada nodo
 * @param vaux: VDinamico<T*> auxiliar para introducir el AVL en él
 */
template<class T>
void AVL<T>::inorden(Nodo<T>* p, int nivel, VDinamico<T*> &vaux ){
    
    if( p ){
        inorden( p->izq, nivel+1, vaux );
        //std::cout << "Procesando nodo " << p->dato << " en el nivel " << nivel << std::endl;
        vaux.insertar( &p->dato );
        inorden( p->der, nivel+1, vaux );
    }
}

/**
 * @brief Método público para calcular la altura del AVL mediante POSTORDEN
 * @return altura del AVL
 */
template<class T>
unsigned int AVL<T>::altura(){
    int mx = 0;
    altura( _raiz, 0, &mx );
    return mx;
}
/**
 * @brief Método privado para calcular la altura del AVL mediante POSTORDEN
 * @param p: nodo raíz desde el que partiremos
 * @param nivel: nivel al que se encuentra cada nodo p
 * @param max: almacena la profundidad máxima encontrada en el AVL, se devuelve por referencia
 */
template<class T>
void AVL<T>::altura(Nodo<T>* p, int nivel, int* max ){
    if( p ){
        altura( p->izq, nivel+1, max );
        altura( p->der, nivel+1, max );
        if( nivel > *max )
            *max = nivel;
    }
}
#endif /* AVL_H */