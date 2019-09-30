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
// Includes
//
#include "map_file.hh"

namespace sys {


	map_file::map_file (std::string & fname)
		: _fd (-1),
		  _size (0),
		  _buf (NULL)
	{
		construct (fname.c_str());
	}

	map_file::map_file (const char * fname)
		: _fd (-1),
		  _size (0),
		  _buf (NULL)
	{
		construct (fname);
	}

	void map_file::construct (const char * const fname)
	{
		stat_t stat (fname);

		if (stat) {
			_fd = ::open(fname, O_RDONLY);
			_size = stat.size();
			//size_t size = ::sysconf(_SC_PAGE_SIZE);

			_buf = (char *) ::mmap(NULL,
								   _size,
								   PROT_READ,
								   MAP_PRIVATE,
								   _fd,
								   0);
			if (_buf == MAP_FAILED) {
				std::string ex ("File '");
				switch (errno) {
				case EACCES:
				case EAGAIN:
				case EBADF:
					logp (sys::e_debug, "Anteriores");
					break;
				case EINVAL:
					logp (sys::e_debug, "EINVAL");
					break;
				case ENFILE:
				default:
					logp (sys::e_debug, "Error mapping file '" << fname << "'.");
					ex += fname;
					ex += "' doesn't exists";
					throw std::invalid_argument(ex);
					break;
				}
				::close (_fd);
				_fd = -1;
			}
		} else {
			std::string ex ("File '");
			ex += fname;
			ex += "' doesn't exists";
			throw std::invalid_argument(ex);
		}
	}

	map_file::~map_file ()
	{
		if (_fd > -1) {
			::close (_fd);
			::munmap (_buf, _size);
		}
	}

} // end namespace sys
