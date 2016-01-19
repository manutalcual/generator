//
// Clase: opt Copyright (c) 2010 Manuel Cano
// Autor: Manuel Cano Muñoz
// Fecha: Thu Sep 30 12:25:12 2010

// Time-stamp: <2010-10-25 16:28:32 manuel>
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

#ifndef opt_hh
#define opt_hh
//
// Includes
//
#include <string.h>
#include <string>
#include <map>
#include <vector>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "common.hh"

namespace sys {
    typedef std::pair<std::string, std::string> pairopt_t;
    typedef std::vector<std::string> vecstring_t;

	class opt
	{
	public:
        enum {
            e_required = 1,
            e_optional = 2,
            e_many = 4,
            e_set = 0x8000
        };

        typedef struct _param_list
        {
            _param_list ()
                : next (NULL)
                {}
            ~_param_list ()
                {
                    if (next)
                        delete next;
                }
            char * param;
            _param_list * next;
        } param_list_t;

        typedef struct _opt_t
        {
        public:
            char _opt_name;
            const char * _long_name;
            param_list_t * _parameters;
            int _flags;
            bool _set;
            const char * _help_string;
            const char * _default;

			bool has_params () { return (_parameters && _parameters->param); }
            const char * param ()
                {
                    if (_parameters && _parameters->param)
                        return _parameters->param;
                    return NULL;
                }
            bool is_set ()
                {
                    return (_flags & e_set);
                }
        } opt_t;

        typedef std::map<std::string, _opt_t *> mapopt_t;
        
		opt (int argc, char ** argv, opt_t opts[]);
        ~opt ();
        operator bool () { return _ok; }
        opt_t * operator [] (std::string path);
        opt_t * operator [] (const char *);

	protected:

	private:
        int _argc;
        char ** _argv;
        opt_t ** _user_opts;
        mapopt_t _opts;
        param_list_t * _param_list;
        bool _ok;

		opt (const opt &) {}

        void intern_options ();
        void parse_options ();
        void asign_parameters ();

        void parse_short_option (int i);
        void parse_long_option (int i);
        void parse_parameter (int i);
        void append_parameter (opt_t * o, char * param);
        void set_short_option (char ch);

        param_list_t * search_first_free_param (opt_t * o);
        param_list_t * search_first_free_param (param_list_t * p);
        void append_parameter_item (param_list_t * p, char * param);

	};

} // end namespace sys

#endif // opt_hh
