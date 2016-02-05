//-*- mode: c++; indent-tabs-mode: t; -*-
//
//! @brief Nombre del proyecto, bloque.
//
// Todos los derechos reservados por By Tech.
// La informaci�n aqu� contenida es propiedad confidencial de la Empresa. 
// La utilizaci�n, divulgaci�n, reproducci�n y distribuci�n, total o parcial, 
// de la misma est� estrictamente prohibida. Salvo acuerdo expreso con By Tech.
// Micro:         Nombre del microcontrolador
// Proyecto:      PY-XXX. Nombre del proyecto
// Program: lamb Copyright (c) 2016 By Tech
// Author: Manuel Cano Mu�oz
// Date: Fri Feb  5 14:08:34 2016
// Time-stamp: <>
// 
// $Id$
//
//
// Includes
//
#include <iostream>


#ifndef lamb_hh
#define lamb_hh


class lamb
{
public:
	lamb ();
	lamb (const lamb & obj);
	~lamb ();
	int the_number ();
	template<typename F>
	void process (F func);
protected:
private:
	int _the_number;
};


#endif // lamb_hh


