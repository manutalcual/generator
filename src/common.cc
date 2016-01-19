//-*- mode: c++; indent-tabs-mode: t; -*-
//
//! @brief Nombre del proyecto, bloque.
//
// Todos los derechos reservados por By Tech.
// La información aquí contenida es propiedad confidencial de la Empresa. 
// La utilización, divulgación, reproducción y distribución, total o parcial, 
// de la misma está estrictamente prohibida. Salvo acuerdo expreso con By Tech.
// Micro:         Nombre del microcontrolador
// Proyecto:      PY-XXX. Nombre del proyecto
// Program: sqler Copyright (c) 2015 Manuel Cano (http://develox.org/)
// Author: manuelcano@by.com.es
// Date: Mon Dec  7 15:45:56 2015
// Time-stamp: <>
// 
// $Id$
//
//
// Includes
//
#include "common.hh"

namespace sys {

    int common::LOG_LEVEL = e_warning;

	common::common ()
	{
	}

	common::~common ()
	{
	}

    std::string itoa (size_t num)
    {
        char ch[124];
        
        ::sprintf (ch, "%lu", num);
        
        return ch;
    }

	std::string lower (std::string str)
	{
		std::string newstring (str);

		for (size_t i = 0; i < str.size(); ++i)
			newstring[i] = ::tolower(newstring[i]);

		return newstring;
	}

	std::string upper (std::string str)
	{
		std::string newstring (str);

		for (size_t i = 0; i < str.size(); ++i)
			newstring[i] = ::toupper(newstring[i]);

		return newstring;
	}

	char * strdup (const char * str, size_t len)
	{
		char * b = new char [len + 1];

		if (! b)
			return NULL;

		::strncpy (b, str, len);

		return b;
	}

	void renove_log_files ()
	{
#ifdef WITH_LOG
		flog ();

		pid_t id = 0;

		id = ::getpid();

		std::string fname;

		fname = itoa(id);
		fname += ".log";

		int fd = ::open (fname.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0);

		if (-1 == fd) {
			logp (sys::e_warning, "Can't create thread log file.");
			return;
		}

		::dup2 (fd, 2);
#endif
	}

    std::string date ()
    {
		time_t t = ::time(0);
		std::string d (::ctime(&t));
		d = d.substr(0, d.size() - 1); // Eliminar el salto de línea
		return d;
    }

    namespace env 
    {
		std::string get (std::string nom)
		{
			std::string r;
			char * pr = ::getenv(nom.c_str());
			if (pr)
				r = pr;
			return r;
		}

		std::string user ()
		{
			return get("USER");
		}

    } // end namespace env


} // end namespace sys

