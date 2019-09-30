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
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <glob.h>
#include <time.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef common_hh
#define common_hh


#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#define ZEROMEM(t,v)    ::memset(&v, 0, sizeof(t));

//
// If we selected log with configure --with-log, define the log
// macros.
//
#ifdef WITH_LOG

#define SET_LOG_LEVEL(x) sys::common::LOG_LEVEL = x;

//
// If we want syslog, define macros to work with syslog.
//
#ifdef USE_SYSLOG

//
// For use with syslog
//
#ifndef LOG_DOMAIN
#warning Log domain undefined. Using PACKAGE_NAME
#endif

#include "syslogp.hh"

#   else // Not syslog, but stderr

#define flog() logp(sys::e_debug, __PRETTY_FUNCTION__)
#define logpn(x,y) if (x <= sys::common::LOG_LEVEL)                     \
        std::cerr														\
			<< "[" << __FILE__											\
			<< ":" << __LINE__	<< "] "									\
			<< y

#define logp(x,y) logpn(x,y) << std::endl
#define logpd(y) std::cerr												\
	<< "[" << __FILE__													\
	<< ":" << __LINE__	<< "] "											\
	<< y

#define nflog()
#define nlogpn(x,y)
#define nlogp(x,y)
#define nlogpd(x)

#   endif // USE_SYSLOG

#else // Without WITH_LOG

#define SET_LOG_LEVEL(x)

#   ifdef USE_SYSLOG

//
// For use with syslog
//
#ifndef LOG_DOMAIN
#warning Log domain undefined. Using PACKAGE_NAME
#endif

//
// Perhaps we want syslog even if we don't want debug log.
//
#include "syslogp.hh"

#   else // Without syslog


#define flog()
#define logpn(x,y)
#define logp(x,y)
#define logpd(x)

#define nflog()
#define nlogpn(x,y)
#define nlogp(x,y)
#define nlogpd(x)

#define SET_LOG_LEVEL(x)

#   endif

#endif // WITH_LOG


namespace sys {

	//
	// Syslog priorities. They will be used when using stderr, and not
	// syslog.
	//
    enum e_log_level {
		e_emerg,
		e_alert,
        e_crit,
        e_err,
        e_warning,
		e_notice,
        e_info,
        e_debug
    };

	char * strdup (const char * str, size_t len);
	std::string lower (std::string str);
	std::string upper (std::string str);
    std::string itoa (size_t num);
    int atoi (std::string num);
	void renove_log_files ();
    std::string date ();

	class common
	{
	public:
        static int LOG_LEVEL;

		common ();
		~common ();
	protected:
	private:
	};

    namespace env
    {
		std::string get (std::string nom);
		std::string user ();

    } // end namespace env

	class file_system
	{
	public:
		static bool safe_mv (const char * const orig_file_name, const char * const dest_file_name)
			{
				glob_t glob;
				std::string path (dest_file_name);
				std::string new_name (dest_file_name);

				path += "*";

				::glob (path.c_str(), 0, NULL, &glob);

				if (glob.gl_pathc > 0) {
					new_name += "." + sys::itoa(glob.gl_pathc);
				}

				return ::rename (orig_file_name, new_name.c_str());
			}
		static bool safe_mv (const std::string & orig_file_name, const std::string & dest_file_name)
			{
				return safe_mv (orig_file_name.c_str(), dest_file_name.c_str());
			}
		static bool mv (const char * const orig_file_name, const char * const dest_file_name)
			{
				return ::rename (orig_file_name, dest_file_name);
			}
		static bool mv (const std::string & orig_file_name, const std::string & dest_file_name)
			{
				return rename (orig_file_name.c_str(), dest_file_name.c_str());
			}
		static bool mkdir (const char * const dir_name)
			{
				return ::mkdir(dir_name, 0755);
			}
		static bool mkdir (const std::string & dir_name)
			{
				return mkdir(dir_name.c_str());
			}
	};

} // end namespace sys

#endif // common_hh
