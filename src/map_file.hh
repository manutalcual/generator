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
// Program: map_file Copyright (c) 2015 By Tech
// Author: Manuel Cano Muñoz
// Date: Thu Dec 10 09:17:37 2015
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
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <string>
#include <stdexcept>

#include "common.hh"
#include "stat.hh"

#ifndef map_file_hh
#define map_file_hh

namespace sys {

	class map_file
	{
	public:
		map_file (const char * fname);
		map_file (std::string & fname);
		~map_file ();

		operator bool () { 	return _fd > -1; }
		size_t size () { return _size; }
		const char * operator [] (size_t position) { return &_buf[position]; }
	private:
		int _fd;
		size_t _size;
		char * _buf;

		void construct (const char * const fname);
	};

} // end namespace sys


#endif // map_file_hh
