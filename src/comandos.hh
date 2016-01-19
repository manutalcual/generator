//
// Clase: comandos Copyright (c) 2016 ByTech
// Autor: Manuel Cano Muñoz
// Fecha: Fri Apr  7 17:06:05 2006
// Time-stamp: <2016-01-18 10:29:11 manuel>
//
// Includes
//
#include <string>
#include "common.hh"

#ifndef comandos_h
#define comandos_h

namespace sys
{
    class IComando
    {
    public:
		virtual ~IComando () {}
		virtual char operator << (char) = 0;
		virtual std::string operator << (std::string) = 0;
		virtual std::string as_string () = 0;
    };

    namespace com
    {
		IComando * factoria (std::string nombre);
    }

    class base_comando : public IComando
    {
		friend IComando * com::factoria (std::string nombre);
    public:
		virtual ~base_comando () {}
		virtual char operator << (char ch) { return ch; }
		virtual std::string operator << (std::string buf) { return buf; }
		virtual std::string as_string () { return std::string (""); }
    protected:
		base_comando ();
    };

    class ucase : public base_comando
    {
    public:
		ucase ();
		ucase (const ucase & c);
		~ucase ();
		virtual char operator << (char ch);
		virtual std::string operator << (std::string buf);
		virtual std::string as_string ();
    private:
		std::string _buf;
    };

    class lcase : public base_comando
    {
    public:
		lcase ();
		lcase (const lcase & c);
		~lcase ();
		virtual char operator << (char ch);
		virtual std::string operator << (std::string buf);
		virtual std::string as_string ();
    private:
		std::string _buf;
    };

} // end namespace sys

#endif // comandos_h


