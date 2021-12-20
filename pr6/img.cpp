/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   img.cpp
 * Author: Alberto Herreros Magaña
 * 
 * Created on 12 de diciembre de 2021, 18:30
 */

#include <sstream>
#include <fstream>
#include <string>
#include "img.h"

using namespace std;

ostream &operator<<(ostream &os, const RGBColor &color) {
    os << "(" << (int)color.rojo() << ", " << (int)color.verde() << ", " << (int)color.azul() << ")";
    return os;
}

Img::Img(int filas, int columnas) {
    this->filas = filas;
    this->columnas = columnas;
    pixels = new RGBColor[filas * columnas];
}


Img::Img(int filas, int columnas, RGBColor &color) {
    this->filas = filas;
    this->columnas = columnas;
    pixels = new RGBColor[filas * columnas];
    for (int i= 0; i< filas*columnas; i++) pixels[i]= color;
} 

Img::Img(const string &fileName) {
    ifstream f(fileName.c_str(), ios::binary);
    
    if (!f) throw ErrorLecturaFichero();
    
    string tmp;
    stringstream stmp;
    
    // Leer cabecera
    getline(f, tmp);
    if (tmp != "P6") throw ErrorFormatoFichero();
    
    // Leer descripción programa generador
    getline(f, tmp);

    // Leer número de filas
    f >> tmp;
    stmp.str(tmp);
    if (!(stmp >> filas)) throw ErrorFormatoFichero();

    // Leer número de columnas
    f >> tmp;
    stmp.clear();
    stmp.str(tmp);
    if (!(stmp >> columnas)) throw ErrorFormatoFichero();
    getline(f, tmp);
    
    // Leer rango de color
    int tamColor;
    f >> tmp;
    stmp.clear();
    stmp.str(tmp);
    if (!(stmp >> tamColor) || (tamColor != 255)) throw ErrorFormatoFichero();
    getline(f, tmp);
    
    // Leer pixels
    int numPixels;
    pixels = new RGBColor[numPixels = filas * columnas];
    RGBColor *auxPixel = pixels;
    unsigned char rgb[3];
    
    while (numPixels--) {
        //if (f.read((char *)rgb, 3) == 0) throw ErrorFormatoFichero();
        f.read( (char*) rgb, 3);
        *(auxPixel++) = RGBColor(rgb[0], rgb[1], rgb[2]);
    }
}

void Img::guardar(const string &fileName) {
    ofstream f(fileName.c_str(), ios::binary);
    
    if (!f) throw ErrorEscrituraFichero();    
    
    f << "P6" << endl;
    f << "# Generada por clase Img" << endl;
    f << filas << " " << columnas << endl;
    f << 255 << endl;
    
    int numPixels = filas * columnas;
    RGBColor *auxPixel = pixels;
    unsigned char rgb[3];
    
    while (numPixels--) {
        rgb[0] = auxPixel->rojo();
        rgb[1] = auxPixel->verde();
        rgb[2] = auxPixel->azul();
        //if (f.write((char *)rgb, 3) == 0) throw ErrorEscrituraFichero();
        f.write((char *)rgb, 3);
        ++auxPixel;
    }
}

void Img::pintarRecuadro (int xmin, int ymin, int xmax, int ymax, int r, int g, int b){    	
	if (xmin > xmax) swap (xmin, ymax);
	if (ymin > ymax) swap (ymin, ymax);

 	for (int i = xmin; i < xmax; ++i) {
		pintarPixel(i, ymin, r, g, b);
		pintarPixel(i, ymax - 1, r, g, b);
	}
	
	for (int i = ymin; i < ymax; ++i) {
		pintarPixel(xmin, i, r, g, b);
		pintarPixel(xmax - 1, i, r, g, b);
	}
}


