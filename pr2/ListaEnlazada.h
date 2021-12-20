/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ListaEnlazada.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 5 de octubre de 2021, 20:03
 */

#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

#include "Nodo.h"

template <class T>
class ListaEnlazada{
    public:
        
        //Clase Iterador
        template <class Y>
        class Iterador{
            public:
                Nodo<Y> *nodo;    

                Iterador( Nodo<Y> *aNodo=0 ): nodo( aNodo ){}

                bool fin(){ return nodo == 0 ;}

                void siguiente(){ nodo = nodo->sig;}

                Y &dato(){ return nodo->dato; }

                Iterador<Y>& operator=( const Iterador<Y> &orig ){ //Operador de asignación
                    nodo=orig.nodo;
                    return *this;
                }
        };
        //Métodos de la clase ListaEnlazada
        ListaEnlazada();
        ListaEnlazada( const ListaEnlazada<T>& orig );
        ListaEnlazada<T>& operator=( const ListaEnlazada<T>& orig );
        
        ~ListaEnlazada();
        
        T& inicio(){ return _cabecera->dato; }
        T& fin(){ return _cola->dato; }

        Iterador<T> iterador() const{ return Iterador<T>(_cabecera); }
        
        void insertaInicio( T &dato );
        void insertaFinal( T &dato );
        void inserta( Iterador<T> &it, T &dato );
        
        void borraInicio();
        void borraFinal();
        void borra( Iterador<T> &it );
        
        int tam(){ return _tam; }
        
        bool buscar( T &dato, Iterador<T> &it );
        
    private:
        Nodo<T> *_cabecera, *_cola;
        int _tam;

};

/**
 * @brief Constructor por defecto de ListaEnlazada: inicio y final apuntan a 0, tam = 0
 */
template<class T>
ListaEnlazada<T>::ListaEnlazada():_cabecera(0),_cola(0),_tam(0){}

template<class T>
ListaEnlazada<T>::ListaEnlazada(const ListaEnlazada<T>& orig){
//    Nodo<T> *p = orig._cabecera;
//    while( p != 0 ){
//        insertaFinal( p->dato );
//        p = p->sig;
//    }
    Iterador<T> it = orig.iterador();
    
    while( !it.fin() ){
        insertaFinal( it.dato() );
        it.siguiente();
    }
    _tam = orig._tam;
}

template<class T>
ListaEnlazada<T>& ListaEnlazada<T>::operator=( const ListaEnlazada<T>& orig ){
    
    while( _cabecera != 0 )
        borraFinal();
    
    Iterador<T> it = orig.iterador();
    
    while( !it.fin() ){
        insertaFinal( it.dato() );
        it.siguiente();
    }
    _tam = orig._tam;
    return *this;
}

template<class T>
ListaEnlazada<T>::~ListaEnlazada(){
    //Nodo<T> *p = _cabecera;
    while( _cabecera != 0 ){ /*p != 0*/
        borraFinal();
        //p = p->sig;
    }
}

template<class T>
void ListaEnlazada<T>::insertaInicio(T& dato){
    
    Nodo<T> *nuevo = new Nodo<T>(dato,_cabecera);
    //¿Lista vacía?
    if( _cola == 0 )
        _cola = nuevo;
    _cabecera = nuevo;  
    
    _tam++;
}

template<class T>
void ListaEnlazada<T>::insertaFinal(T& dato){
    Nodo<T> *nuevo = new Nodo<T>(dato,0);
    //¿Existe un dato en la cola?
    if ( _cola != 0 )
        _cola->sig = nuevo;
    //¿Lista vacía?
    if( _cabecera == 0 )
        _cabecera = nuevo;
    _cola = nuevo;
    
    _tam++;
}

template<class T>
void ListaEnlazada<T>::inserta(Iterador<T>& it, T& dato){
    Nodo<T> *anterior = 0;
    
    if( _cabecera != _cola ){
        anterior = _cabecera;
        while( anterior->sig != it.nodo )
            anterior = anterior->sig;
    }
    Nodo<T> *nuevo = new Nodo<T>( dato, anterior );
    
    anterior->sig = nuevo;
    if( _cabecera == 0 )
        _cabecera = _cola = nuevo;  
    
    _tam++;
}

template<class T>
void ListaEnlazada<T>::borraInicio(){
    
    Nodo<T> *borrado = _cabecera;
    
    _cabecera = _cabecera->sig;
    delete borrado;
    
    if( _cabecera == _cola )
        _cola = 0;
    
    _tam--;
}

template<class T>
void ListaEnlazada<T>::borraFinal(){
    
    Nodo<T> *anterior = 0;
    
    if( _cabecera != _cola ){
        anterior = _cabecera;
        while( anterior->sig != _cola )
            anterior = anterior->sig;
    }
    delete _cola;
    _cola = anterior;
    
    if( anterior != 0 )
        anterior->sig = 0;
    else
        _cabecera = 0;
    
    _tam--;
}

template<class T>
void ListaEnlazada<T>::borra(Iterador<T>& it){
    
    Nodo<T> *anterior = 0;
    
    if( _cabecera != _cola ){
        anterior = _cabecera;
        while( anterior->sig != it.nodo )
            anterior = anterior->sig;
    }
    anterior->sig = it.nodo->sig;
    delete it.nodo;
    
    _tam--;
}

template<class T>
bool ListaEnlazada<T>::buscar(T& dato, Iterador<T>& it){
    
    Nodo<T> *p = _cabecera;
    
    while( p ){
        
        if( p->dato == dato ){
//            Iterador<T> i(p);
//            it = i;
            it.nodo = p;
            return true;       
        }
        p = p->sig;
    }
    return false;
}
#endif /* LISTAENLAZADA_H */

