/***************************************************************************
 *   Copyright (C) 2006 by Antonio J. Rueda   *
 *   ajrueda@coppino   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <ctime>
#include "fecha.h"

const unsigned Fecha::diasMes[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Fecha::Fecha()
{
    time_t tiempoActual;
    struct tm fechaActual;

    time(&tiempoActual); // Obtener hora actual

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
  	localtime_s(&fechaActual, &tiempoActual); 
#else
  	localtime_r(&tiempoActual, &fechaActual); // POSIX  
#endif

    leerTiempo(fechaActual);
}

Fecha::Fecha(unsigned aDia, unsigned aMes, unsigned aAnio, unsigned aHora, unsigned aMin)
{
    // Filtrado de fechas incorrectas
    comprobarFecha(aDia, aMes, aAnio, aHora, aMin);
    dia = aDia;
    mes = aMes;
    anio = aAnio;
    hora = aHora;
    min = aMin;
}

void Fecha::asignarDia(unsigned aDia, unsigned aMes, unsigned aAnio)
{
    comprobarFecha(aDia, aMes, aAnio, hora, min);
    dia = aDia;
    mes = aMes;
    anio = aAnio;
}

void Fecha::asignarHora(unsigned aHora, unsigned aMin)
{
    comprobarFecha(dia, mes, anio, aHora, aMin);
    hora = aHora;
    min = aMin;
}

bool Fecha::operator<(const Fecha &f)
{
    if (anio < f.anio)
	return true;
    else if (anio > f.anio)
	return false;

    if (mes < f.mes)
	return true;
    else if (mes > f.mes)
	return false;

    if (dia < f.dia)
	return true;
    else if (dia > f.dia)
	return false;

    if (hora < f.hora)
	return true;
    else if (hora > f.hora)
	return false;

    if (min < f.min)
	return true;

    return false;
}

Fecha &Fecha::operator=(const Fecha &f)
{
    dia = f.dia;
    mes = f.mes;
    anio = f.anio;
    hora = f.hora;
    min = f.min;
    return *this;
}

void Fecha::anadirMin(int numMin)
{
    struct tm fecha;

    escribirTiempo(fecha);
    fecha.tm_min += numMin;
    mktime(&fecha);
    leerTiempo(fecha);
}

void Fecha::anadirHoras(int numHoras)
{
    struct tm fecha;

    escribirTiempo(fecha);
    fecha.tm_hour += numHoras;
    mktime(&fecha);
    leerTiempo(fecha);
}

void Fecha::anadirDias(int numDias)
{
    struct tm fecha;

    escribirTiempo(fecha);
    fecha.tm_mday += numDias;
    mktime(&fecha);
    leerTiempo(fecha);
}

void Fecha::anadirMeses(int numMeses)
{
    struct tm fecha;

    escribirTiempo(fecha);
    fecha.tm_mon += numMeses;
    mktime(&fecha);
    leerTiempo(fecha);
}

void Fecha::anadirAnios(int numAnios)
{
    struct tm fecha;

    escribirTiempo(fecha);
    fecha.tm_year += numAnios;
    mktime(&fecha);
    leerTiempo(fecha);
}

string Fecha::cadenaDia() const
{
    char buffer[11];
    snprintf(buffer, 11, "%u/%u/%u", dia, mes, anio);
    return string(buffer);
}

string Fecha::cadenaHora() const
{
    char buffer[6];
    snprintf(buffer, 6, "%u:%u", hora, min);
    return string(buffer);
}

Fecha::~Fecha()
{
}

void Fecha::comprobarFecha(unsigned aDia, unsigned aMes, unsigned aAnio, unsigned aHora, unsigned aMin) const
{
    if (aMin > 59 || aHora > 23)
	throw ErrorFechaIncorrecta();
    if (aMes < 1 || aMes > 12)
	throw ErrorFechaIncorrecta();
    if (aDia < 1 || aDia > diasMes[aMes - 1])
	throw ErrorFechaIncorrecta();
    if (aDia == 29 && aMes == 2 && (aAnio % 4 != 0 || (aAnio % 100 == 0 && aAnio % 400 != 0)))
	throw ErrorFechaIncorrecta();
}

void Fecha::leerTiempo(const tm &t)
{
    dia = t.tm_mday;
    mes = t.tm_mon + 1;
    anio = t.tm_year + 1900;
    hora = t.tm_hour;
    min = t.tm_min;
}

void Fecha::escribirTiempo(tm &t)
{
    t.tm_mday = dia;
    t.tm_mon = mes - 1;
    t.tm_year = anio - 1900;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = 0;
}

ostream &operator<<(ostream &os, const Fecha &f)
{
    os << f.cadena();
    return os;
}
