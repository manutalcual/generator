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
// Program: sqler Copyright (c) 2015 Manuel Cano (http://develox.org/)
// Author: manuelcano@by.com.es
// Date: Mon Dec  7 15:45:56 2015
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

#include <iostream>

//#include <QxOrm.h>

#include "common.hh"
#include "opt.hh"
#include "base_parser.hh"
#include "conf.hh"
#include "file.hh"
#include "stat.hh"
#include "map_file.hh"
#include "generator.hh"
#include "parser.hh"

#define log_err(x) std::cerr << "Error: " \
	<< x \
	<< "; at L:" \
	<< _line \
	<< ", C:" \
	<< _col \
	<< std::endl

namespace app {

	void print_tables (sys::conf::block_t * element);
	void print_fields (sys::conf::block_t::mapblock_t & subelements);

	class qmake
	{
	public:
		class cant_create : public ex
		{
		public:
			cant_create (const char * msg,
						 const char * file,
						 int line)
				: ex (msg, file, line)
				{ }
		};

		qmake (sys::conf & pconf);
		bool make_project () throw (qmake::cant_create);
	private:
		sys::conf & _conf;

	};

	void use (char ** argv);

} // end namespace app

