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


sys::opt::opt_t options[] = {
    {
        'h', "help", NULL, sys::opt::e_optional, 0,
        "Display this help", NULL
    },
    {
        'f', "conf-file", NULL, sys::opt::e_required, 0,
        "Configuration file to load.", "hulk.conf"
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
			logp (sys::e_crit, "Must give a configuration file.");
			::exit (-1);
		}

		std::string fname;

		if (opts["conf-file"]->is_set())
			fname = opts["conf-file"]->param();
		else
			fname = "hulk.conf";

		sys::stat_t stat_conf (fname);
		if (! stat_conf) {
			app::use (argv);
			std::cerr << "There is no such file '"
					  << fname
					  << "'."
					  << std::endl;
			::exit (-1);
		}

		sys::map_file map (fname);
		sys::conf conf (map[0]);

		if (! conf.parse()) {
			logp (sys::e_debug, "Error parsing configuration file.");
			return -1;
		}

		sys::conf::block_t * b = conf.find("sys");

		logp (sys::e_debug, "System: "
			  << b->name
			  << ".");

		sys::conf::block_t * main = conf.find("sys/main");
		if (! main)
			throw "Can'f find 'sys/main' element in main.cc.";

		sys::conf::block_t * sub = main->subelements["subprojects"];
		sys::conf::block_t::mapblock_t::
			iterator itb = sub->subelements.begin();
		sys::conf::block_t::mapblock_t::
			iterator ite = sub->subelements.end();


		for (; itb != ite; ++itb) {
			auto data = itb->second;
			if (data->values["type"] == "lib") {
				std::string name = data->values["name"];
				std::string lib_name = data->values["gen"];
				auto lib = conf.find("sys/" + lib_name);

				sys::parser proto ("../etc/hulk.proto.plantilla",
								   lib->name,
								   *lib);

				auto begin = lib->subelements.begin();
				auto end = lib->subelements.end();
				for (; begin != end; ++begin) {
					auto item = begin->second;
					logp (sys::e_debug, "Class: '" << item->name << "'.");
					/*
					sys::parser header ("../etc/entity.header.plantilla",
										item->name,
										*item);
										*/
					sys::parser body ("../etc/entity.body.plantilla",
									  item->name,
									  *item);
									  
					//std::cout << header.resultado() << std::endl;
					std::cout << body.resultado() << std::endl;
				}
				std::cout << proto.resultado() << std::endl;
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
			std::cout << "\t--"
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

