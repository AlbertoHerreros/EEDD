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
#ifndef FECHA_H
#define FECHA_H

/**
@author Antonio J. Rueda
*/

#include <string>
using namespace std;

/** @brief Excepci—n que representa fechas incorrectas */
class ErrorFechaIncorrecta {};

/** @brief Clases sencilla para representar fechas y horas */
class Fecha {
  /** Fecha y hora */
  unsigned dia, mes, anio, hora, min;
	/** Dias por mes */
	static const unsigned diasMes[12];

	/** Comprobaci—n de validez de una fecha */
	void comprobarFecha(unsigned aDia, unsigned aMes, unsigned aAnio, unsigned aHora, unsigned aMin) const;
	/** Funci—n auxiliar de conversi—n desde estructura de tiempo tm de time.h */
	void leerTiempo(const tm &t);
	/** Funci—n	auxiliar de conversi—n a estructura de tiempo tm de time.h */
	void escribirTiempo(tm &t);

public:
	/** Crea una fecha con la hora actual */
	Fecha();
	/** Crea una fecha concreta. Devuelve una excepción ErrorFechaIncorrecta si la fecha introducida no es correcta */
	Fecha(unsigned aDia, unsigned aMes, unsigned aAnio, unsigned aHora = 0, unsigned aMin = 0);
	/** Constructor copia */
	Fecha(const Fecha &f) : 
		dia(f.dia), mes(f.mes), anio(f.anio), hora(f.hora), min(f.min) {}

	/** Asignar un nuevo día */
	void asignarDia(unsigned aDia, unsigned aMes, unsigned aAnio);
	/** Asignar una nueva hora */
	void asignarHora(unsigned aHora, unsigned aMin);

	/** Obtener hora */
	unsigned verHora() const { return hora; }
	/** Obtener minutos */
	unsigned verMin() const { return min; }
	/** Obtener dia */
	unsigned verDia() const { return dia; }
	/** Obtener mes */
	unsigned verMes() const { return mes; }
	/** Obtener a–o */
	unsigned verAnio() const { return anio; }

	/** Obtener una cadena con el d’a */
	string cadenaDia() const;
	/** Obtener una cadena con la hora */
	string cadenaHora() const;
	/** Obtener una cadena con d’a y hora */
	string cadena() const {
		return cadenaDia() + " " + cadenaHora(); 
	}

	/** Indica si las dos fechas tienen el mismo d’a */
	bool mismoDia(const Fecha &f) const {
		return dia == f.dia && mes == f.mes && anio == f.anio; 
	}
	/** Comparar fechas */
	bool operator<(const Fecha &f);
	/** Asignar fechas */
	Fecha &operator=(const Fecha &f);

	/** A–adir un nœmero de minutos indicado */
	void anadirMin(int numMin);
	/** A–adir un nœmero de horas indicado*/
	void anadirHoras(int numHoras);
	/** A–adir un nœmero de d’as indicado */
	void anadirDias(int numDias);
	/** A–adir un nœmero de meses indicado */
	void anadirMeses(int numMeses);
	/** Añadir un nœmero de a–os indicado */
	void anadirAnios(int numAnios);

	~Fecha();
};

/** Serializaci—n de fechas: salida */
ostream &operator<<(ostream &os, const Fecha &f);

#endif
