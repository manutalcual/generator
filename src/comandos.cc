//
// Clase: comandos Copyright (c) 2016 ByTech
// Autor: Manuel Cano Muñoz
// Fecha: Fri Apr  7 17:06:05 2006
// Time-stamp: <2016-01-19 22:02:32 manuel>
//
//
// Includes
//
#include "comandos.hh"

namespace sys
{
    namespace com
    {
		IComando * factoria (std::string nombre)
		{
			if (nombre == "ucase")
				return new ucase;
			else if (nombre == "lcase")
				return new lcase;
			return NULL;
		}
    }

    base_comando::base_comando ()
    {
    }

    ucase::ucase ()
    {
    }

    ucase::ucase (const ucase & c)
		: _buf (c._buf)
    {
    }

    ucase::~ucase ()
    {
    }

    char ucase::operator << (char ch)
    {
		_buf += ::toupper(ch);
		return ch;
    }

    std::string ucase::operator << (std::string buf)
    {
		std::string b = sys::upper(buf);
		_buf += b.c_str();
		return _buf;
    }

    std::string ucase::as_string ()
    {
		return _buf;
    }

    lcase::lcase ()
    {
    }

    lcase::lcase (const lcase & c)
		: _buf (c._buf)
    {
    }

    lcase::~lcase ()
    {
    }

    char lcase::operator << (char ch)
    {
		_buf += ::toupper(ch);
		return ch;
    }

    std::string lcase::operator << (std::string buf)
    {
		std::string b = sys::lower(buf);
		_buf += b.c_str();
		return _buf;
    }

    std::string lcase::as_string ()
    {
		return _buf;
    }

} // end namespace sys
