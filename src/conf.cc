//
// Clase: conf Copyright (c) 2010 Manuel Cano
// Autor: Manuel Cano Muñoz
// Fecha: Wed Sep 15 13:07:05 2010

// Time-stamp: <2016-01-27 23:17:42 manuel>
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
#include "conf.hh"


namespace sys {

	conf::conf (std::string data)
        : base_parser (data),
          _ok (true),
          _released (false),
          _first (NULL),
          _last (_first),
          _scope (0),
          _parsed (false)
	{
        nflog ();
        nlogp (sys::e_debug, "Data to parse:");
        nlogp (sys::e_debug, _data);
	}

    conf::~conf ()
	{
        if (! _released)
            delete _first;
	}

    conf::operator bool ()
    {
        return _ok;
    }

    bool conf::parse ()
    {
        nflog ();

        if (_parsed)
            return _ok;
        //
        // Normally there are comments at begining of file, so begin
        // removing comments.
        //
        nlogp (sys::e_debug, "Skip first comments.");
        skip_comments ();

        nlogp (sys::e_debug, "Begin with parse itself.");

		parse_block ();

        if (_first)
            block (_first);

        _parsed = true;
        return _ok;
    }

    conf::block_t * conf::find (std::string bname)
    {
        nflog ();

		nlogp (sys::e_debug, "Looking for path: " << bname);
        block_t * b = find(bname, _first);
		if (b) {
			nlogp (sys::e_debug, "Name found: " << b->name);
		} else {
			nlogp (sys::e_debug, "Name not found!");
		}
		return b;
    }

    conf::block_t * conf::find (std::string bname, conf::block_t * b)
    {
        nflog ();
        block_t * ret = NULL;

        size_t p = bname.find("/");
		std::string fname;

        if (p != std::string::npos) {
            fname = bname.substr(0, p);
        } else {
            fname = bname;
		}
		nlogp (sys::e_debug, "Looking for: " << fname
			  << " in " << b->name);

        block_t::mapblock_t::iterator itb = b->subelements.begin();
        block_t::mapblock_t::iterator ite = b->subelements.end();

        for (; itb != ite; ++itb) {
            nlogp (sys::e_debug, "Block name: " << itb->second->name
                  << "; looking for: " << fname);

            if ((*itb)->name == fname) {
                nlogp (sys::e_debug, "This is the name.");
                size_t p = bname.find("/");
                if (p != std::string::npos) {
                    bname = bname.substr(p + 1);
                    nlogp (sys::e_debug, "Next name to look for: "
                          << bname);
                    ret = find(bname, *itb);
                } else {
                    ret = *itb;
                }
            }
        }

		return ret;
    }

    std::string conf::operator [] (const char * name)
    {
        return _first->values[name];
    }

    void conf::parse_block ()
    {
        nflog ();
        _first = new block_t;
        _last = _first;
        _first->name = "configuration";
        do {
            parse_block_body ();
            nlogp (sys::e_debug, "char at pos: " << _data[_idx] << ".");
        } while (! match('}') && _idx < _size);
    }

    void conf::parse_block_name ()
    {
        nflog ();

        skip_blanks ();
        _last->name = get_name();
        nlogp (sys::e_debug, "Block name: " << _last->name);
        skip_blanks ();
    }

    void conf::parse_block_body ()
    {
        nflog ();

        nlogp (sys::e_debug, "char at pos: " << _data[_idx] << ".");
        skip_blanks ();
        nlogp (sys::e_debug, "char at pos: " << _data[_idx] << ".");
        std::string bname = get_name();
        skip_blanks ();

        nlogp (sys::e_debug, "Body name: " << bname);

        if (bname == "") {
            nlogp (sys::e_crit, "Block body name is empty. '"
                  << bname << "'");
            throw "Block body name is empty.";
        }

        if (match(':')) {
            incr (); // ++_idx; // skip ':'
            std::string val = get_value();
            nlogp (sys::e_debug, "Inserting: " 
                  << bname << "=" << val
                  << ", into " << _last->name);
            _last->values[bname] = val;
        } else if (match('{')) {

            incr (); //++_idx; // skip '{'
            ++_scope;

            nlogp (sys::e_debug, "Adding new to: " << _last->name
                  << "; the new is: " << bname);

            block_t * newblock = new block_t;
            newblock->name = bname;
            newblock->parent = _last;
            
            _last->subelements.push_back (newblock);
            _last = newblock;

            do {
                parse_block_body ();
            } while (! match ('}'));

            _last = _last->parent;

            incr (); //++_idx; // skip '}'
            --_scope;
		} else if (match('/')) {
			if (is_comment())
				skip_comments ();
        } else if (_idx >= _size) {
            return;
        } else {
            logp (sys::e_crit, "conf parser was expecting "
                  << "':' or '}', but the char at pos is: '"
                  << _data[_idx]
                  << "'; can't continue parsing. "
                  << _idx);
            throw "Conf file not correct.";
        }

        nlogp (sys::e_debug, "char at pos: " << _data[_idx] << ". "
              << _idx);
        skip_blanks ();
    }

    std::string conf::get_name ()
    {
        nflog ();
        std::string val;

        while (_idx < _size 
               && (::isalnum(_data[_idx]) || _data[_idx] == '_'))
        {
            val += _data[_idx];
            incr ();
        }

        nlogp (sys::e_debug, val);

        return val;
    }

    std::string conf::get_value ()
    {
        nflog ();
        std::string value;

        skip_blanks ();
        if (_data[_idx] == '"') {
            incr (); //++_idx; // skip '"'
            while (_idx < _size && _data[_idx] != '"') {
                value += _data[_idx];
                incr ();
            }
            incr (); //++_idx; // skip '"'
            skip_blanks ();
            if (! match(';')) {
                nlogp (sys::e_crit, "conf parser was expecting "
                      << "';', but the char at pos is: '"
                      << _data[_idx]
                      << "'; can't continue parsing.");
                throw "Semicolon not found after a value.";
            }
            incr (); //++_idx; // skip ';'
        } else {
            while (_idx < _size && _data[_idx] != ';') {
                value += _data[_idx];
                incr ();
            }
            if (! match(';')) {
                nlogp (sys::e_crit, "conf parser was expecting "
                      << "';', but the char at pos is: '"
                      << _data[_idx]
                      << "'; can't continue parsing.");
                throw "Semicolon not found after a value.";
            }
            incr (); //++_idx; // skip ';'
        }

        nlogp (sys::e_debug, value);

        return value;
    }

    bool conf::match (char ch)
    {
        if (_data[_idx] != ch) {
            return false;
        }

        return true;
    }

    bool conf::is_comment ()
    {
		if (_data[_idx] == '/' && _data[_idx + 1] == '*') {
			do {
				++_idx;
			} while (! (_data[_idx] == '*' && _data[_idx + 1] == '/'));
			_idx += 2; // skip '*/'
			return false;
		}
		
        if (_data[_idx] == '/' && _data[_idx + 1] == '/')
            return true;

        return false;
    }

    void conf::skip_blanks ()
    {
        nflog ();
        base_parser::skip_blanks ();

        while (is_comment()) {
            skip_to ('\n');
            ++_idx; // skip '\n' itself
			base_parser::skip_blanks ();
        }
    }

    void conf::skip_comments ()
    {
        nflog ();
        while (is_comment())
            skip_blanks ();
    }

    void conf::block (block_t * f)
    {
        block_t::mapblock_t::iterator itb = f->subelements.begin();
        block_t::mapblock_t::iterator ite = f->subelements.end();

        for (; itb != ite; ++itb) {
            nlogp (sys::e_debug, "Block name: " << itb->second->name);
            if ((*itb)->subelements.size())
                block (*itb);
        }
    }

    void conf::subblock (block_t * f)
    {
        (void) f; // avoid warning unused
#if 0
        nlogp (sys::e_debug, "Sub-block name (next): " << f->name);

        if (f->follow) {
			nlogp (sys::e_debug, "following node: "
				  << f->follow->name);
            block (f->follow);
		}

        block_t * sf = f->next;

        if (f->next) {
			nlogp (sys::e_debug, "sub node: " << f->next->name);
            subblock (f->next);
		}
#endif
    }

} // end namespace sys

