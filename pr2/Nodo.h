/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Nodo.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 5 de octubre de 2021, 20:11
 */

#ifndef NODO_H
#define NODO_H

template<class T>
class Nodo{
    public:
        T dato;
        Nodo *sig;
        
        Nodo( T &aDato, Nodo *aSig = 0):
        dato(aDato), sig(aSig){}
};


#endif /* NODO_H */

