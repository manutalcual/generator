//
// Clase: opt Copyright (c) 2010 Manuel Cano
// Autor: Manuel Cano Muñoz
// Fecha: Thu Sep 30 12:25:12 2010

// Time-stamp: <2010-10-25 16:12:36 manuel>
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
#include "opt.hh"

namespace sys {

	opt::opt (int argc, char ** argv, opt::opt_t opts[])
        : _argc (argc),
          _argv (argv),
          _user_opts (&opts),
          _param_list (NULL),
          _ok (true)
    {
		nflog ();
        intern_options ();
        parse_options ();
        asign_parameters ();
    }

	opt::~opt ()
	{
        if (_param_list)
            delete _param_list;
	}

    opt::opt_t * opt::operator [] (std::string path)
    {
		nlogp (sys::e_debug, "operator: " << path);
        return _opts[path];
    }

    opt::opt_t * opt::operator [] (const char * path)
    {
		nlogp (sys::e_debug, "operator: " << path);
        return _opts[path];
    }

    void opt::intern_options ()
    {
        nflog ();
        opt_t * o = _user_opts[0];

        for (; o->_long_name; ++o) {
            nlogp (sys::e_debug, "Name: " << o->_long_name);
            _opts[o->_long_name] = o;
        }
    }

    void opt::parse_options ()
    {
		nflog ();

        for (int i = 1; i < _argc; ++i) {
            if (_argv[i][0] == '-' && _argv[i][1] == '-') {
                parse_long_option (i);
            } else if (_argv[i][0] == '-' && _argv[i][1] != '-'
                       && _argv[i][1]) {
                parse_short_option (i);
            } else if (_argv[i][0] != '-' && _argv[i][0]) {
                parse_parameter (i);
            }
        }
    }

    void opt::asign_parameters ()
    {
        nflog ();

        opt_t * o = _user_opts[0];
        param_list_t * p = _param_list;

        for (; o->_long_name; ++o) {
			nlogp (sys::e_debug, "Option: " << o->_long_name);

            if ((o->_flags & e_required) && !(o->_flags & e_set))
                _ok = false;

            if ((o->_flags & e_many) && (o->_flags & e_set)) {

                param_list_t * pl = p;

				if (! pl) {
					nlogp (sys::e_debug, "There is no parameter list!");
				}

                for (; pl && pl->next; pl = pl->next) {
					nlogp (sys::e_debug, "Append parameter: " << pl->param);
                    append_parameter (o, pl->param);
				}

                if (pl) {
					nlogp (sys::e_debug, "Append parameter: " << pl->param);
                    append_parameter (o, pl->param);
					p = pl->next;
				}


			} else if ((o->_flags & e_required) && (o->_flags & e_set)) {
                
                param_list_t * pl = p;

				if (! pl) {
					nlogp (sys::e_debug, "There is no parameter list!");
				}

                if (pl) {
					nlogp (sys::e_debug, "Append parameter: " << pl->param);
                    append_parameter (o, pl->param);
					p = pl->next;
				}


            } else if ((o->_flags & e_optional) && (o->_flags & e_set)) {

                if (!o->_parameters) {
                    if (p) {
                        o->_parameters = new param_list_t;
                        o->_parameters->param = p->param;
                        p = p->next;
                    }
                }

            }
        }
    }

    void opt::parse_short_option (int i)
    {
        nflog ();
		char * argv = &_argv[i][1]; // assume it begins with "-"
        int count = ::strlen(argv);

        for (int i = 0; i < count; ++i) {
            set_short_option (argv[i]);
        }
    }

    void opt::parse_long_option (int i)
    {
        nflog ();
		char * argv = &_argv[i][2]; // assume it begins with "--"
        char * equal = ::strchr(argv, '=');

        if (equal) {
            *equal = '\0';
            ++equal;
        }
		opt_t * o = _opts[argv];

		if (o) {
			o->_flags |= e_set;

            if (equal)
                append_parameter (o, equal);

		} else {
			nlogp (sys::e_warning, argv << " isn't known, ignoring it");
		}
    }

    void opt::parse_parameter (int i)
    {
        nflog ();

        if (_param_list) {
            param_list_t * np = search_first_free_param(_param_list);
            append_parameter_item (np, _argv[i]);
        } else {
            _param_list = new param_list_t;
            _param_list->param = _argv[i];
        }
    }

    void opt::append_parameter (opt_t * o, char * param)
    {
        nflog ();

        if (o->_parameters) {
            if (o->_flags & e_many) {
                param_list_t * p = search_first_free_param(o);
                append_parameter_item (p, param);
            } else {
                nlogp (sys::e_debug, "This option do not accept "
                      "multiple parameters.");
                nlogp (sys::e_debug, "Ignoring '" << param << "'.");
            }
        } else {
			nlogp (sys::e_debug, "appending parameter: " << param
				  << " in " << o->_long_name);
            o->_parameters = new param_list_t;
            o->_parameters->param = param;
        }
    }

    void opt::set_short_option (char ch)
    {
        nflog ();
        opt_t * o = _user_opts[0];

        for (; o->_long_name; ++o) {
            if (o->_opt_name == ch) {
                o->_flags |= e_set;
                break;
            }
        }
    }

    opt::param_list_t * opt::search_first_free_param (opt_t * o)
    {
        nflog ();

        param_list_t * pl = o->_parameters;

        for (; pl && pl->next; pl = pl->next)
            ;

        return pl;
    }

    opt::param_list_t * opt::search_first_free_param (opt::param_list_t * p)
    {
        nflog ();

        param_list_t * pl = p;

        for (; pl && pl->next; pl = pl->next)
            ;

        return pl;
    }

    void opt::append_parameter_item (param_list_t * p, char * param)
    {
        nflog ();

		nlogp (sys::e_debug, "appending parameter: " << param);

        p->next = new param_list_t;
        p->next->param = param;
    }


} // end namespace sys

