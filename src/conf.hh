//
// Clase: conf Copyright (c) 2010 Manuel Cano
// Autor: Manuel Cano Muñoz
// Fecha: Wed Sep 15 13:07:05 2010

// Time-stamp: <2016-01-18 15:12:31 manuel>
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

#ifndef conf_hh
#define conf_hh
//
// Includes
//
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "common.hh"
#include "file.hh"
#include "base_parser.hh"

namespace sys {

	class conf : public net::base_parser
	{
	public:
        typedef std::map<std::string, std::string> mapstring_t;
        class block_t
        {
        public:
            typedef std::map<std::string, block_t *> mapblock_t;
            block_t ()
                : parent (NULL)
                {}
            block_t * parent;
            std::string name;
            mapstring_t values;
            mapblock_t subelements;
        };

		conf (std::string data);
		~conf ();
        operator bool ();
        bool parse ();
        block_t * find (std::string bname);
        std::string operator [] (const char * name);
	protected:
	private:
        bool _ok;
        bool _released;
        block_t * _first;
        block_t * _last;
        int _scope;
        bool _parsed;

        void parse_block ();
        void parse_block_name ();
        void parse_block_body ();

        std::string get_name ();
        std::string get_value ();

        bool match (char ch);
        bool is_comment ();
        void skip_blanks ();
        void skip_comments ();

        void block (block_t * bl);
        void subblock (block_t * bl);
		block_t * find (std::string bname, block_t * b);

        inline void incr ()
            {
                if (++_idx >= _size)
                    throw "End of file found.";
            }
	};

} // end namespace sys

#endif // conf_hh

