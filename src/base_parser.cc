//
// Clase: base_parser Copyright (c) 2010 SISDETEC
// Autor: Manuel Cano Muñoz
// Fecha: Mon Aug 30 13:22:40 2010

// Time-stamp: <2010-10-06 15:00:07 manuel>
//
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
//
//
// Includes
//
#include "base_parser.hh"

namespace net {

	base_parser::base_parser (std::string data)
        : _idx (0),
          _data (data),
          _size (_data.size())
	{
	}

	base_parser::~base_parser ()
	{
	}

    void base_parser::skip_blanks ()
    {
        nflog ();
        while (_idx < _size && 
               (_data[_idx] == ' '
                || _data[_idx] == '\t'
                || _data[_idx] == '\r'
                || _data[_idx] == '\n'
                || _data[_idx] == '\v')) {
            ++_idx;
        }
    }

    void base_parser::skip_to (char ch)
    {
        while (_idx < _size && (_data[_idx] != ch))
            ++_idx;
    }

} // end namespace net

