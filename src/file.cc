//
// Clase: file Copyright (c) 2010 SISDETEC
// Autor: Manuel Cano Muñoz
// Fecha: Mon Aug 30 13:53:13 2010

// Time-stamp: <2010-11-05 08:59:22 manuel>
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
#include "file.hh"

namespace sys {

	file::file (std::string name)
        : _name (name),
          _fd (NULL),
          _size (0),
          _buf (NULL),
          _ok (true)
	{
        flog ();
        stat_t st (_name.c_str());

        logp (sys::e_debug, "Stat: " << _name << ", " << st.size());
        
        if (st) {
            _size = st.size();
            _fd = ::fopen(_name.c_str(), "rb");
        } else {
            _fd = NULL;
            _size = 0;
            _ok = false;
        }

        if (_fd) {
            logp (sys::e_debug, "Reading data into buffer.");
            int r = 0;
            _buf = new char [_size + 1];

            while (r < _size) {
                r += fread(_buf, 1, _size, _fd);
                logp (sys::e_debug, "Readed: " << r);
            }

            _buf[_size] = '\0';
        }
	}

	file::~file ()
	{
        delete [] _buf;
	}

    std::string file::data ()
    {
        if (_buf)
            return _buf;

        return "";
    }

} // end namespace sys

