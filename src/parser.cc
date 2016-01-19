//
// Clase: parser Copyright (c) 2016 ByTech
// Autor: Manuel Cano Muñoz
// Fecha: Wed Mar 15 16:29:27 2006
// Time-stamp: <2016-01-19 20:27:16 manuel>
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
          _ok (false),
          _str (""),
          _i (0),
          _size (0),
          _clase (clase),
		  _count (0),
		  _current_count (0)
    {
		logp (sys::e_debug, "File name: " << file_name << ".");

		sys::file f (file_name.c_str());

        _str = f.data();
        _size = f.size();
        logp (sys::e_debug, "El tamaño del archivo es de " << _size << " bytes.");

        _ok = analiza();
        logp (sys::e_debug, "Resultado:");
        logp (sys::e_debug, "Se han recorrido " << _i << " bytes.");
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
		flog ();
        do
        {
            switch (_str[_i])
            {
            case '$':
                _buf += escapa(&_bloque, _i);
                break;
			case '%':
				word = captureField(blk, i);
				break;
            case '\\':
                _buf += escapaComando(&_bloque, _i);
                break;
            default:
                _buf += _str[_i++];
                break;
            }
        }
        while (_str[_i] != '\0');

        return true;
    }

	std::string parser::escapa (sys::conf::block_t * blk, int & i)
	{
		flog ();
		std::string word;

		if (_str[i] == '$') {
			word = escapa(i);
		} else if (_str[i] == '%') {
			logp (sys::e_debug, "capture field");
			word = captureField(blk, i);
		} else {
			word = captureWord(i);
            logp (sys::e_debug, "*** Error *** escaping nothing. '"
                  << _str[i]
                  << "'.");
		}

		return word;
	}

    std::string parser::escapa (int & i)
    {
		flog ();
        std::string ret;
        std::string word;

        ++i; // Saltar el '$'
        word = captureWord(i);

        if (word == "usuario") {
            ret = sys::env::user();
        }
        else if (word == "fecha") {
            ret = sys::date();
        }
        else if (word == "item") {
			logp (sys::e_debug, "Se encuentra item.");
            ret = _clase;
        } else if (word == "") {
            ret = "";
		} else {
            ret = sys::env::get(word);
            if (ret == "")
            {
                ret = _bloque.values[word];
            }
        }

        nlogp (sys::e_debug, "Variable escapada: '" << ret << "'");
        return ret;
    }

    std::string parser::skipComando (int & i)
    {
        std::string com ("");
        int llaves = 1; // Llaves de apertura

        while (i < _size && _str[i] != '{')
        {
            com += _str[i];
            ++i;
        }
        com += _str[i];
        ++i;

        while (i < _size && llaves)
        {
            if (_str[i] == '}') 
            {
                // Con la llave de cierre,
                // decrementamos la cuenta de llaves :)
                --llaves;
            }
            else if (_str[i] == '{')
            {  
                // Con la llave de apertura,
                // incrementamos la cuenta de
                // llaves :(
                ++llaves;
            }
            com += _str[i];
            ++i;
        }

        return com;
    }

    std::string parser::escapaComando (sys::conf::block_t * blk, int & i)
    {
		flog ();
        std::string ret;
        std::string word;
        std::string res;

        logp (sys::e_debug, "Escapa comando: '" << _str[i] << "'.");
        ++i; // Skip '\'
        if (i < _size && _str[i] != '\\')
        {
            logp (sys::e_debug, "Escapa comando: '" << _str[i] << "'.");
            word = captureWord(i);

            if (word == "for")
            {
				_scopes.push_back (_current_count);
				_current_count = 0;

                ++i; // Saltar espacio en blanco
                ret = parseComando(i);
                logp (sys::e_debug, "For: " << ret);
                res += comandoFor(blk, ret, i);

				_current_count = _scopes.back();
				_scopes.pop_back ();
            }
            else if (word == "do")
            {
                ++i; // Saltar espacio en blanco
                ret = parseComando(i);
                logp (sys::e_debug, "Do: " << ret);
                res += comandoDo(blk, sys::com::factoria(ret), i);
            }
            else if (word == "while")
            {
                ++i; // Saltar espacio en blanco
                ret = parseComando(i);
                logp (sys::e_debug, "While: " << ret);
                res += comandoFor(blk, ret, i);
            }
			else if (word == "if")
			{
				logp (sys::e_debug, "If detected. '" << _str[i] << "'.");
				std::string op1;
				std::string ope;
				std::string op2;
				
				skipBlanks (i);

				logp (sys::e_debug, "Go to the if.");

				op1 = escapa(blk, i);
				skipBlanks (i);
				ope = captureOperator(i);
				skipBlanks (i);
				op2 = escapa(blk, i);
				skipBlanks (i);

				logp (sys::e_debug, "If '"
					  << op1
					  << "' "
					  << ope
					  << " '"
					  << op2
					  << "'.");

				if (resolveOperator(op1, ope, op2)) {
					skipBlanks (i);
					res += bloque(blk, i);
					skipBlanks (i);
					logp (sys::e_debug, "Despues del bloque if: '"
						  << _str[i]
						  << "'.");
					if (_str[i] == '{') {
						skipBloque (i);
                    }
				} else {
					skipBlanks (i);
					skipBloque (i);
					skipBlanks (i);
					if (_str[i] == '{') {
                        std::string c;
						c = bloque(blk, i);
                        res += c;
                        logp (sys::e_debug, "Captured en else: \n'"
                              << c
                              << "'");
                        logp (sys::e_debug, "Fin else '"
                              << _str[i]
                              << "'.");
                    }
				}
			}
            else
            {
                ++i; // Saltar espacio en blanco
                ret = parseComando(i);
                logp (sys::e_debug, "Unknown: '" << ret << "'");

                std::cerr << "Error: comando " << word 
                          << " no implementado." << std::endl;
                std::string com;

                com = skipComando(i);

                std::cerr << "El comando es el siguiente: " << std::endl;
                std::cerr << com << std::endl;

                res += "\n/*\n * Error: comando '";
                res += word;
                res += "' desconocido.\n";
                res += " *\n";
                res += " * Texto del comando.\n";
                res += " *\n";
                res += com;
                res += "\n */\n";
            }
        }
        else
        {
            res += '\\';
            ++i;
        }

        return res;
    }

    std::string parser::parseComando (int & i)
    {
        std::string nom;

        if (_str[i] == '$') {
            ++i; // Skip '$'
            nom = captureWord(i);
        } else {
            while ((i < _size) && (::isalnum(_str[i]) || _str[i] == '_'))
            {
                nom += _str[i];
                ++i;
            }
        }

        return nom;
    }

    std::string parser::comandoFor (sys::conf::block_t * blk,
                                    std::string nom,
									int & i)
    {
		flog ();
        int pos = 0;
        bool encontrado = false; // Si el nombre del comando esta en
        // el bloque
        std::string ret;
        sys::conf::block_t::mapblock_t::iterator ib = blk->subelements.begin();
        sys::conf::block_t::mapblock_t::iterator ie = blk->subelements.end();
        sys::conf::block_t::mapblock_t::iterator sib;
        sys::conf::block_t::mapblock_t::iterator sie;


		skipBlanks (i);

        // Guardamos la posicion del inicio del bloque, despues de
        // saltar el '{'
        logp (sys::e_debug, "comandoFor: '" << _str[i] << "'.");
        pos = ++i;
        logp (sys::e_debug, "comandoFor: '" << _str[i] << "'.");

		logp (sys::e_debug, "Buscando '" << nom << "'.");
        if (nom != "item") {

            for (; ib != ie; ++ib) {
                logp (sys::e_debug, "Bloque: " << ib->second->name);
                if (ib->second->name == nom) {
                    encontrado = true;
                    break;
                }
            }

            logp (sys::e_debug, "...");

            //
            // Tratar el comando desconocido
            //
            if (! encontrado) {
                skipBloque (i);
                return "";
            }

            sib = ib->second->subelements.begin();
            sie = ib->second->subelements.end();

        } else {
            
            sib = blk->subelements.begin();
            sie = blk->subelements.end();

        }

		logp (sys::e_debug, "....");
		
        logp (sys::e_debug, "Bloque encontrado: " << ib->second->name);

        if (sib == sie) {
            logp (sys::e_debug, "El bloque esta vacio.");
			skipBloque (i);
            return "";
        }

        for (_current_count = 1; sib != sie; ++sib, ++_current_count)
        {
            int brackets = 0;
            pos = i;
            logp (sys::e_debug, "Sub bloque: '" << sib->second->name << "'.");
            while (pos < _size && !brackets  && _str[pos] != '}') {
                // Poner valor
                if (_str[pos] == '%') {
                    if (_str[pos + 1] == '?') {
                        logp (sys::e_debug, "Interrogante. '"
                              << _str[pos]
                              << "'.");
                        std::string word;
						++pos; // Skip '%'
                        ++pos; // Skip '?'
                        word = parseComando(pos);
                        logp (sys::e_debug, "Comando: '" << word << "': '"
                              << _str[pos]
                              << "'.");

                        logp (sys::e_debug, "Distance: "
                              << std::distance(sib, sie)
                              << ".");
                        
                        if (word == "last") {
							sys::conf::block_t::mapblock_t::iterator tmp = sib;
                            if (++tmp == sie) {
                                logp (sys::e_debug, "\tfin de bloque '"
                                      << _str[pos]
                                      << "'.");
                                ret += bloque(sib->second, pos);
                            } else {
                                logp (sys::e_debug, "\tno es fin de bloque '"
                                      << _str[pos]
                                      << "'.");
                                skipBloque (pos);
                            }
                        } else if (word == "nolast") {
							sys::conf::block_t::mapblock_t::iterator tmp = sib;
                            if (++tmp != sie) {
                                logp (sys::e_debug, "\tno fin de bloque '"
                                      << _str[pos]
                                      << "'.");
                                ret += bloque(sib->second, pos);
                            } else {
                                logp (sys::e_debug, "\tes fin de bloque. '"
                                      << _str[pos] << "'.");
                                skipBloque (pos);
                            }
                        } else {
                            while (_str[pos] != '}')
                                ret += _str[pos++];
                        }
                    } else {
						ret += captureField(sib->second, pos);
                    }
                }
                // Los comandos se pueden anidar
                else if (_str[pos] == '\\')
                {
                    //++pos; // Saltar el '\'
                    ret += escapaComando(sib->second, pos);
                    logp (sys::e_debug, "Returning from nested command 1 '"
                          << _str[pos] << "'.");

                    ++pos; // Skip closing '}'
                    logp (sys::e_debug, "Returning from nested command 2 '"
                          << _str[pos] << "'.");
                }
                // Y se pueden poner variables de entorno dentro
                // de los bloques
                else if (_str[pos] == '$')
                {
                    ret += escapa(pos);
                }
                // Lo demas lo copiamos verbatim
                else
                {
                    if (_str[i] == '{') {
                        logp (sys::e_debug, "Incrementing brackets");
                        ++brackets;
                    } else if (_str[i] == '}') {
                        logp (sys::e_debug, "Decrementing brackets");
                        --brackets;
                    }

                    ret += _str[pos];
                }
                ++pos;
            }
        }
		i = pos;

        logp (sys::e_debug, "Saliendo del bloque FOR. '" << _str[i] << "'.");
        return ret;
    }

	std::string parser::bloque (sys::conf::block_t * blk,
								int & pos)
	{
		flog ();
        std::string ret;


        while (pos < _size && _str[pos] != '{')
            ++pos;

        // Saltar el '{'
        ++pos;

        while (pos < _size && _str[pos] != '}')
        {
            // Los comandos se pueden anidar
            if (_str[pos] == '\\')
            {
                ++pos; // Saltar el '\'
                ret += escapaComando(blk, pos);
            }
            // Y se pueden poner variables de entorno dentro
            // de los bloques
            else if (_str[pos] == '$' || _str[pos] == '%')
            {
                ret += escapa(blk, pos);
            }
            // Lo demas lo copiamos verbatim
            else
            {
                ret += _str[pos];
            }
            ++pos;
        }

		++pos; // Skip last '}'

        logp (sys::e_debug, "Bloque captured:\n"
              << ret);

        return ret;
	}

    std::string parser::comandoDo (sys::conf::block_t * blk,
                                   sys::IComando * pcom,
								   int & pos)
    {
		flog ();
		sys::IComando & com = *pcom; // Para usar la sintaxis de operadores
        // el bloque
        std::string ret;


        while (pos < _size && _str[pos] != '{')
            ++pos;

        // Saltar el '{'
        ++pos;

        while (pos < _size && _str[pos] != '}')
        {
            // Los comandos se pueden anidar
            if (_str[pos] == '\\')
            {
                ++pos; // Saltar el '\'
                ret += escapaComando(blk, pos);
            }
            // Y se pueden poner variables de entorno dentro
            // de los bloques
            else if (_str[pos] == '$')
            {
                std::string es;
                com << escapa(pos);
            }
            // Lo demas lo copiamos verbatim
            else
            {
                com << _str[pos];
            }
            ++pos;
        }
        ret += com.as_string();
        delete pcom;
        pcom = NULL;

        return ret;
    }

	std::string parser::captureField (sys::conf::block_t * blk, int & i)
	{
		flog ();
		std::string field;
		std::string value;

		++i; // skip '%'
		logp (sys::e_debug, "captureField: '"
			  << _str[i]
			  << "'.");
		switch (_str[i]) {
		case '.':
			field = captureWord(++i);
			value = blk->values[field];
			break;
		case '#':
			++i; // Skip '#'
			field = "#count";
			value = sys::itoa(_current_count);
			break;
		case '\\':
			field = "#char";
			value = _str[++i];
			++i;
			break;
		case '?':
			field = captureWord(i);
			value = resolveQuestion(blk, ++i);
			break;
		default:
			field = "#field";
			value = blk->name;
			break;
		}

		logp (sys::e_debug, "Field: '" << blk->name << "'.");
		logp (sys::e_debug, "\t'" << field << "': '"
			  << value
			  << "'.");

		return value;
	}

	// no usada
	bool parser::resolveQuestion (sys::conf::block_t * blk, int & i)
	{
		flog ();
		std::string question;
		bool ret = false;

		question = captureWord(i);
		if (question == "last") {
			ret = false;
		} else if (question == "nolast") {
			ret = false;
		}

		return ret;
	}

	std::string parser::captureWord (int & i)
	{
		std::string word;

		while (i < _size && (::isalnum(_str[i]) || _str[i] == '_'))
			word += _str[i++];

		logp (sys::e_debug, "Captured word: '" << word << "'.");

		return word;
	}

	std::string parser::captureOperator (int & i)
	{
		std::string word;
		
		while (i < _size && _str[i] != ' ')
			word += _str[i++];

		logp (sys::e_debug, "Captured operator: '" << word << "'.");
		
		return word;
	}
	
	bool parser::resolveOperator (std::string op1, std::string ope, std::string op2)
	{
		if (ope == "==")
			return op1 == op2;
		else if (ope == "<=")
			return op1 <= op2;
		else if (ope == ">=")
			return op1 >= op2;
		else if (ope == "!=")
			return op1 != op2;
		return false;
	}
	
	void parser::skipBlanks (int & i)
	{
		while (i < _size && ::isblank(_str[i]))
			++i;
	}
	
	void parser::skipBloque (int & i)
	{
		flog ();
		int llaves = 1;
		logp (sys::e_debug, "Saltando bloque '" << _str[i] << "'.");
		++i; // Skip '{'
		while (i < _size && llaves)
		{
			logp (sys::e_debug, "\t '" << _str[i] << "'.");
			if (_str[i] == '}') {
				--llaves;
			} else if (_str[i] == '{') {  
				++llaves;
			}
			++i;
		}
		logp (sys::e_debug, "Saltando bloque '" << _str[i] << "'.");
	}
	
} // end namespace sys


