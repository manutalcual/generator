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
        "Configuration file to load.", "generator.conf"
    },
    {
        'd', "conf-dir", NULL, sys::opt::e_required, 0,
        "Directory to load templates from.", "$HOME/etc"
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
		logp (sys::e_debug, "Parameters readed, continuing.");
		if (opts["help"]->is_set()) {
			app::use (argv);
			::exit (0);
		}

		if (! opts) {
			logp (sys::e_crit, "Must give a configuration file.");
			::exit (-1);
		}

		std::string dname;
		if (opts["conf-dir"]->is_set()) {
			dname = opts["conf-dir"]->param();
		} else {
			const char * path = getenv("HOME");
			dname += path;
			dname += "/etc";
		}

		std::string fname;
		if (opts["conf-file"]->is_set())
			fname = dname + "/" + opts["conf-file"]->param();
		else
			fname = dname + "/generator.conf";

		sys::stat_t stat_conf (fname);
		if (! stat_conf) {
			app::use (argv);
			std::cerr << "There is no such file '"
					  << fname
					  << "'."
					  << std::endl;
			::exit (-2);
		}

		sys::map_file map (fname);
		sys::conf conf (map[0]);

		if (! conf.parse()) {
			logp (sys::e_debug, "Error parsing configuration file.");
			::exit (-3);
		}

		sys::conf::block_t * b = conf.find("sys");

		logp (sys::e_debug, "System: "
			  << b->name
			  << ".");

		sys::conf::block_t * main = conf.find("sys/main/subprojects");
		if (! main)
			throw "Can't find 'sys/main' element in conf file.";

		sys::conf::block_t::mapblock_t::iterator itb = main->subelements.begin();
		sys::conf::block_t::mapblock_t::iterator ite = main->subelements.end();

		logp (sys::e_debug, "Going through elements.");
		for (; itb != ite; ++itb) {
			sys::conf::block_t * data = *itb;
			if (data->values["type"] == "lib") {
				logp (sys::e_debug, "Found element " << data->values["name"] << " in list.");
				std::string name = data->values["name"];
				std::string lib_name = data->values["source"];
				sys::conf::block_t * lib = conf.find("sys/" + lib_name);
				sys::conf::block_t * templates = conf.find("sys/" + lib_name + "/templates");

				logp (sys::e_debug, "Going through subelements of " << name << ".");

				logp (sys::e_debug, "  Templates of " << lib->name << ": ");
				sys::conf::block_t::mapblock_t::iterator tmpl = templates->subelements.begin();
				sys::conf::block_t::mapblock_t::iterator tmple = templates->subelements.end();
				for ( ; tmpl != tmple; ++tmpl) {

					logp (sys::e_debug, "   template " << (*tmpl)->name << ".");
					logp (sys::e_debug, "     header " << (*tmpl)->values["header"] << ".");

					sys::conf::block_t::mapblock_t::iterator bobjs = lib->subelements.begin();
					sys::conf::block_t::mapblock_t::iterator eobjs = lib->subelements.end();
					++bobjs; // skip 'templates' element
					logp (sys::e_debug, "  Go to '" << lib->name << "'");
					for ( ; bobjs != eobjs; ++bobjs) {
						logp (sys::e_debug, "     Subelement: '" << (*bobjs)->name << "'");
						if ((*tmpl)->values["header"] != "") {
							sys::parser body_text (dname + "/" + (*tmpl)->values["header"],
																		 (*bobjs)->name,
																		 *(*bobjs));

							std::string bname ("" + (*bobjs)->name + (*tmpl)->values["header_extension"]);
							sys::stat_t stat_file (bname);
							if (stat_file) {
								sys::file_system::safe_mv (bname, bname + ".old");
							}

							logp (sys::e_debug, "File to create: " << bname << ".");
							std::ofstream body (bname.c_str(), std::ios::out);
							if (! body.is_open()) {
								logp (sys::e_crit, "NO BODY FILE!!! " << bname);
								throw "Can't create body file";
							}

							body << body_text.resultado();
							std::cout << body_text.resultado() << std::endl;
						}

						if ((*tmpl)->values["body"] != "") {
							sys::parser body_text (dname + "/" + (*tmpl)->values["body"],
																		 (*bobjs)->name,
																		 *(*bobjs));

							std::string bname ("" + (*bobjs)->name + (*tmpl)->values["body_extension"]);
							sys::stat_t stat_file (bname);
							if (stat_file) {
								sys::file_system::safe_mv (bname, bname + ".old");
							}

							logp (sys::e_debug, "File to create: " << bname << ".");
							std::ofstream body (bname.c_str(), std::ios::out);
							if (! body.is_open()) {
								logp (sys::e_crit, "NO BODY FILE!!! " << bname);
								throw "Can't create body file";
							}

							body << body_text.resultado();
							std::cout << body_text.resultado() << std::endl;
						}
					}
				}
			}
		}
	} catch (const char * err) {
		std::cerr << "Exception: " << err << "." << std::endl;
	} catch (char * const err) {
		std::cerr << "Exception: " << err << "." << std::endl;
	} catch (const std::string &  err) {
		std::cerr << "Exception: " << err << "." << std::endl;
	} catch (const app::generator::create & err) {
		std::cerr << "Exception: (create) " << err.what() << "." << std::endl;
	} catch (const app::generator::file & err) {
		std::cerr << "Exception: (file) " << err.what() << "." << std::endl;
		} catch (const std::exception & err) {
		std::cerr << "Exception (std): '" << err.what() << "'." << std::endl;
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
