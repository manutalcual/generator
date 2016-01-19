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
// Program: stat Copyright (c) 2015 By Tech
// Author: Manuel Cano Mu�oz
// Date: Thu Dec 10 09:19:37 2015
// Time-stamp: <>
// 
// $Id$
//
//
// Includes
//
// Includes
//
#include "stat.hh"

namespace sys {

	stat_t::stat_t (const char * const fname)
		: _file_exists (false),
		  _size (0),
		  _fname (fname)
	{
		construct ();
	}

	stat_t::stat_t (std::string & fname)
		: _file_exists (false),
		  _size (0),
		  _fname (fname)
	{
		construct ();
	}

	bool stat_t::stat ()
	{
		return _file_exists;
	}

	void stat_t::construct ()
	{
		struct stat st;
		// Returns zero on success (file exists)
		_file_exists = ! ::stat(_fname.c_str(), &st);

		_size = st.st_size;
	}

} // end namespace sys


