//
// Clase: base_parser Copyright (c) 2010 SISDETEC
// Autor: Manuel Cano Muñoz
// Fecha: Mon Aug 30 13:22:40 2010

// Time-stamp: <2010-09-06 12:20:05 manuel>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//   02110-1301	USA

#ifndef base_parser_hh
#define base_parser_hh
//
// Includes
//
#include <stdlib.h>
#include <string>
#include <map>

#include "common.hh"

namespace net {

	class base_parser
	{
	public:
		virtual ~base_parser ();
	protected:
        int _idx;
        std::string _data;
        int _size;

		base_parser (std::string data);
        virtual void skip_blanks ();
        virtual void skip_to (char ch);
	private:
	};

} // end namespace net

#endif // base_parser_hh

