/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   img.h
 * Author: Alberto Herreros Magaña
 *
 * Created on 12 de diciembre de 2021, 18:30
 */

#ifndef IMG_H
#define IMG_H

#include "img.h"
#include <string>
#include <iostream>

using namespace std;

/** Error al abrir un fichero para lectura */
class ErrorLecturaFichero {};
/** Formato incorrecto (no es una imagen PPM) */
class ErrorFormatoFichero {};
/** Error al abrir un fichero para escritura */
class ErrorEscrituraFichero {};

/** Color con tres componentes: rojo, verde, negro codificadas en 3 bytes */
class RGBColor {
    unsigned char r, g, b;
    
public:
    /** Iniciar a negro */
    RGBColor() { r = g = b = 0; }
    
    /** Iniciar al color indicado */
    RGBColor(unsigned char r, unsigned char g, unsigned char b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    
    /** Indica si ambos colores son iguales */
    bool operator==(RGBColor &color) {
        return color.r == r && color.g == g && color.b == b;
    }
    
    /** Indica si se trata de distinto colores */
    bool operator!=(RGBColor &color) {
        return !(*this == color);
    }
        
    /** Obtener componente rojo */
    unsigned char rojo() const { return r; }
    /** Obtener componente verde */
    unsigned char verde() const { return g; }
    /** Obtener componente azul */
    unsigned char azul() const { return b; }
};

/** Operador de salida a stream */
ostream &operator<<(ostream &os, const RGBColor &color);

/** Clase para leer imágenes en formato PPM */
class Img {
    /** Pixels codificados en rgb */
    RGBColor *pixels;
    /** Tamaño de la imagen */
    int filas, columnas;
    
public:
    /** Constructor para una imagen vacía del tamaño indicado */
    Img(int filas, int columnas);
    
    /** Constructor para una imagen vacía al tamaño y color RGB indicado */
    Img(int filas, int columnas, RGBColor &color);
    
    /** Constructor para leer la imagen de un fichero PPM indicado */
    Img(const string &fileName);
    
    /** Acceder al color situado en la posición (x, y) */
    RGBColor &pixel(int x, int y) { return pixels[y * filas + x]; }
    void pintarPixel(int x, int y, char r, char g, char b) {
    	RGBColor color(r,g,b);
    	pixels[y * filas + x] = color;
    }
    
    /** Pinta un punto grande en las coordenadas (x,y) en la imagen */
    void pintarPixelGrande(int x, int y, char r, char g, char b) {
   		pintarPixel (x,y,r,g,b);
   		pintarPixel (x,y+1,r,g,b);
   		pintarPixel (x+1,y,r,g,b);
   		pintarPixel (x+1,y+1,r,g,b);    
    }

	/** Pinta un recuadro con las coordenadas de las esquinas indicadas */
    void pintarRecuadro(int xmin, int ymin, int xmax, int ymax, int r, int g, int b);

    /** Guardar la imagen en un fichero PPM */
    void guardar(const string &fileName);
    
    /** Numero de filas */
    int numFilas() { return filas; }
    /** Numero de columnas */
    int numColumnas() { return columnas; }
    /** Pinta un recuadro de color azul en la imagen **/
};

#endif /* IMG_H */

