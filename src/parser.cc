//
// Clase: parser Copyright (c) 2016 ByTech
// Autor: Manuel Cano Muñoz
// Fecha: Wed Mar 15 16:29:27 2006
// Time-stamp: <2016-01-27 23:46:25 manuel>
//
//
// Includes
//
#include "parser.hh"

namespace sys {

    parser::parser (std::string file_name,
					std::string clase,
                    sys::conf::block_t & bl)
        : _bloque (bl),
		  _file (file_name),
		  _size (_file.size()),
          _str (_file[0]),
          _ok (false),
          _i (0),
          _clase (clase),
		  _buf (""),
		  _cur_scope (0),
		  _index (0)
    {
        logp (sys::e_debug, "==========================================");
		logp (sys::e_debug, "File name: " << file_name << ".");
        logp (sys::e_debug, "El tamaño del archivo es de " << _size << " bytes.");
		logp (sys::e_debug, "Pidiendo análisis.");

		// First scope
		
		scope_t scope;
		scope.block = &_bloque;
		scope.begin = _bloque.subelements.begin();
		scope.end = _bloque.subelements.end();
		scope.count = _bloque.subelements.size();
		scope.index = 1;
		_scopes.push_back (scope);
		
        _ok = analiza();

        logp (sys::e_debug, "Se han recorrido " << _i << " bytes.");
        nlogp (sys::e_debug, "Resultado:");
        nlogp (sys::e_debug, _buf);
        logp (sys::e_debug, "==========================================");
    }

    parser::~parser ()
    {
    }

    parser::operator bool ()
    {
        return _ok;
    }

    std::string & parser::resultado ()
    {
        return _buf;
    }

	bool parser::analiza ()
	{
		_buf += escapa(_i);
		return true;
	}

	std::string parser::escapa (int & i)
	{
		std::string word;
		
		word = escapa(i, '\0');

		return word;
	}

	std::string parser::escapa (int & i, char ch)
	{
		std::string word;
		
		do {
			switch (_str[i]) {
			case '\\':
				word += escape_slash(++i);
				break;
			case '$':
				word += escape_var(++i);
				break;
			case '%':
				word += escapa_field(++i);
				break;
			default:
				word += _str[i++];
				break;
			}
		} while (i < _size && (_str[i] != '\0' && _str[i] != ch));

		return word;
	}

	std::string parser::escapa_field (int & i)
	{
		std::string word;
		std::string command;

		if (_str[i] == '?') {
			word = "?";
			++i;
			command = capture_word(i);
		} else if (_str[i] == '.') {
			++i;
			command = capture_word(i);
			word = _scopes[_cur_scope].block->values[command];
            if (word == "") {
                word = "." + command;
            }
		} else if (_str[i] == '#') {
			std::string num;
			logp (sys::e_debug, "Field num: ("
				  << _cur_scope << ") "
				  << _scopes[_cur_scope].index);
			int s = _scopes[_cur_scope].index; 
			++i;
			if (_str[i] == '+') {
				++i;
				skip_between (i);
				num = capture_num(i);
				s += sys::atoi(num);
			}
			word = sys::itoa(s);
		} else {
			word = _scopes[_cur_scope].block->name;
		}

		return word;
	}

	std::string parser::escape_slash (int & i)
	{
        std::string word;
		std::string command;
		
		if (_str[i] == '\\') {
			word = "\\";
			++i;
		} else {
			command = capture_word(i);
			word = process_command(command, i);
		}

		return word;
	}

	std::string parser::process_command (std::string command, int & i)
	{
		std::string word;
		
		skip_blanks (i);
		if (command == "for") {
			word = process_for(i);
		} else if (command == "do") {
			word = process_do(i);
			skip_blanks (i);
		} else if (command == "if") {
			word = process_if(i);
			skip_blanks (i);
		} else {
			word = command;
		}

		return word;
	}

	std::string parser::process_if (int & i)
	{
		std::string subelem;
		std::string word;
		std::string op1;
		std::string oper;
		std::string op2;

		skip_blanks (i);
		op1 = escapa(i, ' ');
		skip_blanks (i);
		oper = capture_oper(i);
		skip_blanks (i);
		op2 = escapa(i, ' ');
		skip_between (i);

		if (oper == "==") {
			if (op1 == op2) {
				run_block (i, word);
				skip_between (i);
				skip_bloque (i);
			} else {
				skip_bloque (i);
				skip_between (i);
				run_block (i, word);
			}
		} else if (oper == "!=") {
			if (op1 != op2) {
				run_block (i, word);
				skip_between (i);
				skip_bloque (i);
			} else {
				skip_bloque (i);
				skip_between (i);
				run_block (i, word);
			}
		}

		return word;
	}

	void parser::run_block (int & i, std::string & word)
	{
		if (_str[i] == '[') {
			++i; // skip '['
			word = escapa(i, ']');
			++i; // skip ']'
		}
	}

	bool parser::look_for_subelement (std::string & element, int & depth)
	{
		tblockite_t beg = _scopes[_cur_scope].block->subelements.begin();
		tblockite_t end = _scopes[_cur_scope].block->subelements.end();;
		bool found = false;

		for (; beg != end; ++beg) {
			tblock_t & item = *(*beg);

			if (item.name == element) {
				scope_t scope;
				scope.block = *beg;
				scope.count = scope.block->subelements.size();
				scope.index = 0;
				push_scope (scope);
				found = true;
				break;
			}
		}

		return found;
	}

	std::string parser::process_for (int & i)
	{
		std::string subelem;
		std::string word;
		int scope_depth = 0;  // we consider this to be our initial
							  // scope, now

		skip_between (i);

		if (_str[i] == '[')
			++i; // skip '[' that's a MUST
		else {
			subelem = capture_word(i);
			skip_between (i);
			++i; // skip '['
			if (!look_for_subelement(subelem, scope_depth)) {
				logp (sys::e_crit, "Subelement '"
					  << subelem
					  << "' not found!");
				--i; // skip_bloque will try to skip '[', so go back
					 // one position
				skip_bloque (i);
				return "";
			}
		}

		logp (sys::e_debug, "en for..... " << _cur_scope);
		// This has to be initialized after looking for subelement
		tblockite_t beg = _scopes[_cur_scope].block->subelements.begin();
		tblockite_t end = _scopes[_cur_scope].block->subelements.end();;
		scope_t & cur_scope = _scopes[_cur_scope];

		cur_scope.index = 0;
		int index = 1;
		int last_pos = i;
		for (; beg != end; ++beg) {
			scope_t scope;
			scope.block = (*beg);
			scope.begin = beg;
			scope.end = end;
			scope.count = scope.block->subelements.size();
			scope.index = index; //++cur_scope.index;
			push_scope (scope);

			logp (sys::e_debug, "");
			logp (sys::e_debug, "        " << scope.block->name
				  << " " << scope.index
				  << " (" << index << ")"
				  << " (" << _scopes[_cur_scope].index << ")");
			++index;
			logp (sys::e_debug, "");
			int pos = i;
			word += escapa(pos, ']');
			last_pos = pos;

			pop_scope ();
		}
		i = last_pos;
		++i; // skip closing ']' a MUST

		if (subelem != "") {
			pop_scope ();
		}

		return word;
	}

	std::string parser::process_do (int & i)
	{
		sys::IComando * pcom = NULL;
		std::string word;
		std::string command;
		std::string tmp;

		command = capture_word(i);
		pcom = sys::com::factoria(command);
		if (pcom) {
			sys::IComando & com = *pcom;

			skip_between (i);
			// TODO verify it is really at block begin
			tmp = escapa(++i, ']');
			++i; // skip '['
			com << tmp;
			word = com.as_string();
		}
		return word;
	}

	std::string parser::capture_word (int & i)
	{
		std::string word;

		do {
			word += _str[i++];
		} while (i < _size && (::isalnum(_str[i]) || _str[i] == '_'));

		return word;
	}

	std::string parser::escape_var (int & i)
	{
		std::string word;

		if (_str[i] == '{') {
			word =  capture_until(i, '}');
		} else {
			word =  capture_word(i);
		}

		if (word == "usuario") {
            word = sys::env::user();
        } else if (word == "fecha") {
            word = sys::date();
        } else if (word == "_") {
            word = _clase;
        } else if (word == "") {
            word = "";
		} else {
            word = sys::env::get(word);
            if (word == "") {
                word = _scopes[_cur_scope].block->values[word];
            }
        }

		return word;
	}

	std::string parser::capture_until (int & i, char ch)
	{
		std::string word;

		++i; // opening char
		do {
			word += _str[i++];
		} while (i < _size && _str[i] != ch);
		++i; // closing char

		return word;
	}

	std::string parser::capture_oper (int & i)
	{
		std::string word;

		do {
			word += _str[i++];
		} while (i < _size && ::ispunct(_str[i]));
		return word;
	}

	std::string parser::capture_num (int & i)
	{
		std::string word;

		do {
			word += _str[i++];
		} while (i < _size && ::isdigit(_str[i]));

		return word;
	}

	void parser::skip_bloque (int & i)
	{
		int llaves = 1;

		// Skip onlo if it is really a code block
		if (_str[i] != '[')
			return;
		
		++i; // Skip '['
		while (i < _size && llaves) {
			if (_str[i] == ']') {
				--llaves;
			} else if (_str[i] == '[') {  
				++llaves;
			}
			++i;
		}
	}

	void parser::skip_blanks (int & i)
	{
		std::string word;
		while (i < _size && (::isblank(_str[i]) /* || _str[i] == '\n' */))
			word += _str[i++];
	}

	void parser::skip_between (int & i)
	{
		while ((i < _size) && (::isblank(_str[i]) || (_str[i] == '\n')))
			++i;
	}

	void parser::push_scope (scope_t & scope)
	{
		logp (sys::e_debug, "Scope count: "
			  << _cur_scope
			  << ", field num: "
			  << _scopes[_cur_scope].index);
		_scopes.push_back (scope);
		++_cur_scope;
	}
	
	void parser::pop_scope ()
	{
		_scopes.pop_back ();
		--_cur_scope;
		logp (sys::e_debug, "Scope count: "
			  << _cur_scope
			  << ", field num: "
			  << _scopes[_cur_scope].index);
	}
	
} // end namespace sys


