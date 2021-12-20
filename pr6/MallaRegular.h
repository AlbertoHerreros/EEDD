/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MallaRegular.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 10 de diciembre de 2021, 18:32
 */

#ifndef MALLAREGULAR_H
#define MALLAREGULAR_H

#include <vector>
#include <list>
#include <stdio.h>
#include <math.h>

template<typename T>
class MallaRegular {
    
public: 
        
    //Clase Casilla
    template<typename Y>
    class Casilla{
        std::list<Y> puntos;
        public:
        friend class MallaRegular<Y>;
        Casilla(): puntos() {}
        void insertar(const Y &dato) { puntos.push_back(dato); }
        
        Y *buscar(const Y &dato){
            typename std::list<T>::iterator it;
            it = puntos.begin();
            for( ; it != puntos.end(); ++it){
                if( *it == dato )
                    return &(*it);
            }
            return 0;
        }
        
        bool borrar(const Y &dato){
             typename std::list<T>::iterator it;
            it = puntos.begin();
            for (;it != puntos.end(); ++it){
                if (*it == dato) {
                    puntos.erase(it);
                    return true;
                }
            }
            return false;
        }
        
        unsigned tam(){
            return puntos.size();
        }
        
        typename std::list<Y>::iterator inicioPuntos(){
            return puntos.begin();
        }
    
        typename std::list<Y>::iterator finPuntos(){
            return puntos.end();
        }
    
        
    };
    
    //Clase MallaRegular
    MallaRegular(int aXMin, int aYMin, int aXMax, int aYMax, int aNDiv);
    MallaRegular& operator=( const MallaRegular &otra );
    void insertar(float x, float y, const T &dato);
    T *buscar(float x, float y, const T &dato);
    bool borrar(float x, float y, const T &dato);
    
    std::vector<T> buscarRadio( float xCentro, float yCentro, float radio);
    float maxElementosPorCelda();
    float promedioElementosPorCelda();
  
private:
    
    float xMin, yMin, xMax, yMax; // Tamaño real global
    float tamaCasillaX, tamaCasillaY; // Tamaño real de cada casilla
    std::vector<std::vector<Casilla<T> > > mr; // Vector 2D de casillas
    int tamMalla = 0; //n elementos que existen en la malla
    int nDiv = 0; //Número de divisiones realizadas

    Casilla<T> *obtenerCasilla(float x, float y);
};


template<typename T>
MallaRegular<T>::MallaRegular(int aXMin, int aYMin, int aXMax, int aYMax, 
int aNDiv) : xMin(aXMin), yMin(aYMin), xMax(aXMax), yMax(aYMax){
    tamaCasillaX = (xMax - xMin)/aNDiv;
    tamaCasillaY = (yMax - yMin)/aNDiv;
    mr.insert(mr.begin(), aNDiv, std::vector<Casilla<T> >(aNDiv));
    nDiv = aNDiv;
}
template<typename T>
MallaRegular<T>& MallaRegular<T>::operator=(const MallaRegular& otra){
    tamMalla = otra.tamMalla;
    mr = otra.mr;
    nDiv = otra.nDiv;
    tamaCasillaX = otra.tamaCasillaX;
    tamaCasillaY = otra.tamaCasillaY;
    xMax = otra.xMax;
    xMin = otra.xMin;
    yMax = otra.yMax;
    yMin = otra.yMin;
    return *this;
}
template<typename T>
MallaRegular<T>::Casilla<T> *MallaRegular<T>::obtenerCasilla (float x, float y){
    int i = (x - xMin) / tamaCasillaX;
    int j = (y - yMin) / tamaCasillaY;
    return &mr[i][j];
}
template<typename T>
void MallaRegular<T>::insertar(float x, float y, const T& dato){
    Casilla<T> *c = obtenerCasilla(x,y);
    c->insertar(dato);
    tamMalla++;
}
template<typename T>
bool MallaRegular<T>::borrar(float x, float y, const T& dato){
    Casilla<T> *c = obtenerCasilla(x,y);
    return c->borrar(dato);
    tamMalla--;
}

/**
 * @brief Función que devuelve un vector con todos los elementos dentro del radio de un círculo formado sobre un punto,
 * se cogen las coordenadas del cuadrado en el que está inscrito y se comparan distancias hasta el centro para comprobar su posición
 * @param xCentro: coordenada x del centro del círculo
 * @param yCentro: coordenada y del centro del círculo
 * @param radio: radio de la circunferencia de búsqueda
 * @return elementos dentro del radio
 */
template<typename T>
std::vector<T> MallaRegular<T>::buscarRadio( float xCentro, float yCentro, float radio){
    
    std::vector<T> v;
    //Límites del área cuadrada de búsqueda
    float xIzq = xCentro - radio;
    float xDer = xCentro + radio;
    float yUp = yCentro + radio;
    float yDown = yCentro - radio;
    //Recorremos el área
    for( float i = xIzq; i <= xDer; i = i + tamaCasillaX ){
        for( float j = yDown; j <= yUp; j = j + tamaCasillaY ){
            
            if( i >= xMin && i <= xMax && j >= yMin && j <= yMax){
                typename std::list<T>::iterator it;
                Casilla<T> *c = obtenerCasilla(i,j);
                
                //Comprobamos la distancia de cada punto
                for( it = c->inicioPuntos(); it != c->finPuntos(); it++ ){

                    float distancia = sqrt( (pow( (xCentro - (*it)->getX() ),2) + 
                    (pow( yCentro - (*it)->getY() ,2) ) ) );
                    //¿Dentro de la circunferencia?
                    if( distancia <= radio ){
                        v.push_back(*it);
                    }  
                } 
            }
        }
    }
   return v; 
}

/**
 * @brief Función que devuelve el promedio de elementos por celda de la MallaRegular
 * @return promedio
 */
template<typename T>
float MallaRegular<T>::promedioElementosPorCelda(){
    
    float nElementos = tamMalla;
    return nElementos/( nDiv*nDiv );
    
}

/**
 * @brief Función que número máximo de elementos registrados en una celda de la MallaRegular
 * @return máximo de elementos encontrados
 */
template<typename T>
float MallaRegular<T>::maxElementosPorCelda(){
    float maximo = 0;
    for( float i = xMin; i < xMax; i = i + tamaCasillaX ){
        for( float j = yMin; j < yMax; j = j + tamaCasillaY ){
            Casilla<T> *casilla = obtenerCasilla(i,j);
            if( casilla->tam() > maximo )
                maximo = casilla->tam();
        }
    }
    return maximo;
}

#endif /* MALLAREGULAR_H */

