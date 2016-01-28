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
#include "main.hh"

#ifdef DEBUG
const char * const CONF_FILE_NAME = "../etc/hulk.conf";
#else
const char * const CONF_FILE_NAME = "/etc/hulk.conf";
#endif

sys::opt::opt_t options[] = {
    {
        'h', "help", NULL, sys::opt::e_optional, 0,
        "Display this help", NULL
    },
    {
        'f', "conf-file", NULL, sys::opt::e_optional, 0,
        "Configuration file to load.", CONF_FILE_NAME
    },
    {
        'p', "conf-path", NULL, sys::opt::e_optional, 0,
        "Internal configuration file path to follow when generating sources.", "sys/main/subprojects"
    },
    {
        'o', "output", NULL, sys::opt::e_optional, 0,
        "Output file name.", "generated_output.txt"
    },
    {
        't', "template", NULL, sys::opt::e_required, 0,
        "Template file to generate code based on.", ""
    },
    {
        NULL
    }
};



int main (int argc, char ** argv)
{
	SET_LOG_LEVEL (sys::e_debug);

	try {

		sys::opt opts (argc, argv, options);

		if (opts["help"]->is_set()) {
			app::use (argv);
			::exit (0);
		}

		if (! opts) {
			logp (sys::e_crit, "Must give a template file.");
			std::cerr << "Must give a template file." << std::endl;
			::exit (-1);
		}

		std::string conf_name;
		std::string conf_path;
		std::string output_name;
		std::string template_file;

		if (opts["conf-file"]->is_set())
			conf_name = opts["conf-file"]->param();
		else
			conf_name = opts["conf-file"]->_default;

		if (opts["conf-path"]->is_set())
			conf_path = opts["conf-path"]->param();
		else
			conf_path = opts["conf-path"]->_default;

		if (opts["output"]->is_set())
			output_name = opts["ouput"]->param();
		else
			output_name = opts["output"]->_default;

		if (opts["template"]->is_set())
			output_name = opts["tempate"]->param();

		logp (sys::e_debug, "Configuration file: " << conf_name << ".");
		logp (sys::e_debug, "Configuration path: " << conf_path << ".");
		logp (sys::e_debug, "Template file:      " << template_file << ".");
		logp (sys::e_debug, "Output file:        " << output_name << ".");

		sys::stat_t stat_conf (conf_name);
		if (! stat_conf) {
			app::use (argv);
			std::cerr << "There is no such file '"
					  << conf_name
					  << "'."
					  << std::endl;
			::exit (-1);
		}

		sys::map_file map (conf_name);
		sys::conf conf (map[0]);

		if (! conf.parse()) {
			logp (sys::e_debug, "Error parsing configuration file.");
			return -1;
		}

		sys::conf::block_t * main = conf.find(conf_path);
		if (! main)
			throw "Can'f find internal configuration path element in "
				"config file (this is in main.cc, just at the beggining).";

		sys::conf::block_t::mapblock_t::
			iterator itb = main->subelements.begin();
		sys::conf::block_t::mapblock_t::
			iterator ite = main->subelements.end();

		for (; itb != ite; ++itb) {
			auto data = *itb;
			sys::parser the_parser (template_file,
									(*itb)->name,
									*(*itb));

			if (opts["output"]->is_set()) {
				std::string pname (output_name);
				sys::stat_t stat_indice (output_name);
				std::ofstream file (output_name);
				if (! file.is_open()) {
					logp (sys::e_crit, "NO OUTPUT FILE!!! (" << output_name << ")");
					throw "Can't create output file";
				}
				file << the_parser.resultado();
			} else {
				std::cout << the_parser.resultado() << std::endl;
			}
		}

		/*
		app::generator gen (conf);

		gen.make_project ();
		*/
	} catch (char * err) {
		std::cerr << "Exception: " << err << "." << std::endl;
	} catch (const char * const err) {
		std::cerr << "Exception: " << err << "." << std::endl;
	} catch (const char * err) {
		std::cerr << "Exception: " << err << "." << std::endl;
	} catch (char * const err) {
		std::cerr << "Exception: " << err << "." << std::endl;
	} catch (const app::generator::create & err) {
		std::cerr << "Exception: (create) " << err.what() << "." << std::endl;
	} catch (const app::generator::file & err) {
		std::cerr << "Exception: (file) " << err.what() << "." << std::endl;
	} catch (...) {
		std::cerr << "Exception: unknown." << std::endl;
	}

	return 0;
}


namespace app {

	void use (char ** argv)
	{
		std::cout << argv[0] << " (C) 2015 By Tech" << std::endl;

		for (int i = 0; options[i]._long_name; ++i) {
			std::cout << "\t -"
					  << (options[i]._opt_name ? options[i]._opt_name : ' ')
					  << "\t--"
					  << (options[i]._long_name ? options[i]._long_name : "")
					  << "=<param> (default: "
					  << (options[i]._default ? options[i]._default : "")
					  << ") "
					  << (options[i]._help_string ? options[i]._help_string : "")
					  << std::endl;
		}
		std::cout << std::endl;
	}

} // end namespace app

