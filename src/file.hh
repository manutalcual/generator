//
// Clase: file Copyright (c) 2010 SISDETEC
// Autor: Manuel Cano Muñoz
// Fecha: Mon Aug 30 13:53:13 2010

// Time-stamp: <2016-01-16 18:24:16 manuel>
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

#ifndef file_hh
#define file_hh
//
// Includes
//
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

#include "common.hh"

namespace sys {

	class file
	{
        class stat_t
        {
        public:
            stat_t (const char * name)
                : _ok (false)
                {
                    ZEROMEM (struct ::stat, _st);
                    if (::stat(name, &_st) == 0)
                        _ok = true;
                }
            operator bool ()
                {
                    return _ok;
                }
            long size ()
                {
                    return _st.st_size;
                }
        private:
            struct ::stat _st;
            bool _ok;
        };
	public:
		file (std::string name);
		~file ();
        operator bool () { return _ok; }
        std::string data ();
		size_t size () { return _size; }
	protected:
	private:
        std::string _name;
        FILE * _fd;
        int _size;
        char * _buf;
        bool _ok;
	};

} // end namespace sys

#endif // file_hh

