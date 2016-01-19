//-*- mode: c++; indent-tabs-mode: t; -*-
//
//! @brief Nombre del proyecto, bloque.
//
// Todos los derechos reservados por By Tech.
// La información aquí contenida es propiedad confidencial de la Empresa. 
// La utilización, divulgación, reproducción y distribución, total o parcial, 
// de la misma está estrictamente prohibida. Salvo acuerdo expreso con By Tech.
// Micro:         Nombre del microcontrolador
// Proyecto:      PY-XXX. Nombre del proyecto
// Program: stat Copyright (c) 2015 By Tech
// Author: Manuel Cano Muñoz
// Date: Thu Dec 10 09:19:37 2015
// Time-stamp: <>
// 
// $Id$
//
//
// Includes
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

#include "common.hh"

#ifndef stat_hh
#define stat_hh


namespace sys {

	class stat_t
	{
	public:
		stat_t (const char * fname);
		stat_t (std::string & fname);
		bool stat ();
		size_t size () { return _size; }
		operator bool () { return _file_exists; }
	private:
		bool _file_exists;
		size_t _size;
		const std::string _fname;

		void construct ();
	};

} // end namespace sys


#endif // stat_hh


