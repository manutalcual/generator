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
        'c', "class", NULL, sys::opt::e_optional, 0,
        "Configuration file to load.", "hulk.conf"
    },
    {
        'o', "output", NULL, sys::opt::e_optional, 0,
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

#if 0 // html
				sys::parser indice_text ("../etc/web.index.plantilla",
										 lib->name,
										 *lib);

				std::string pname ("generated/index.html");
				sys::stat_t stat_indice (pname);

				std::ofstream indice (pname);
				if (! indice.is_open()) {
					logp (sys::e_crit, "NO INDICE FILE!!! " << pname);
					throw "Can't create indice file";
				}

				std::cout << indice_text.resultado() << std::endl;
				indice << indice_text.resultado();
#endif

#if 0 // protos
				sys::parser proto_text ("../etc/hulk.proto.plantilla",
										lib->name,
										*lib);

				std::string pname ("generated/hulk.proto");
				//sys::stat_t stat_proto (pname);

				//if (stat_proto) {
				//	sys::file_system::safe_mv (pname, pname + ".old");
				//}

				std::ofstream proto (pname);
				if (! proto.is_open()) {
					logp (sys::e_crit, "NO PROTO FILE!!! " << pname);
					throw "Can't create proto file";
				}

				std::cout << proto_text.resultado() << std::endl;
				proto << proto_text.resultado();

				return 0;
#endif

				auto begin = lib->subelements.begin();
				auto end = lib->subelements.end();
				for (; begin != end; ++begin) {
					auto item = begin->second;
					logp (sys::e_debug, "Class: '" << item->name << "'.");
#if 0 // business
					/* business header
					sys::parser header_text ("../etc/business.header.plantilla",
											 item->name,
											 *(item->subelements.begin()->second));

					std::string hname ("generated/" + sys::lower(item->name) + "businessbase.h");
					sys::stat_t stat_header (hname);

					//if (stat_header) {
					//	sys::file_system::safe_mv (hname, hname + ".old");
					//}

					std::ofstream header (hname);
					if (! header.is_open()) {
						logp (sys::e_crit, "NO HEADER FILE!!! " << hname);
						throw "Can't create header file";
					}

					header << header_text.resultado();
					std::cout << header_text.resultado() << std::endl;
					*/
					
					/* business body
					 */
					sys::parser body_text ("../etc/test.body.template",
										   item->name,
										   *item);
									  

					std::string bname ("generated/test_" + sys::lower(item->name) + "businessbase.cpp");
					sys::stat_t stat_body (bname);

					//if (stat_body) {
					//	sys::file_system::safe_mv (bname, bname + ".old");
					//}

					std::ofstream body (bname);
					if (! body.is_open()) {
						logp (sys::e_crit, "NO BODY FILE!!! " << bname);
						throw "Can't create body file";
					}

					body << body_text.resultado();
					std::cout << body_text.resultado() << std::endl;
					break;
#endif
#if 0 // html
					sys::parser body_text ("../etc/web.plantilla",
										   item->name,
										   *item);
									  

					std::string bname ("generated/" + sys::lower(item->name) + ".html");
					sys::stat_t stat_body (bname);

					std::ofstream body (bname);
					if (! body.is_open()) {
						logp (sys::e_crit, "NO BODY FILE!!! " << bname);
						throw "Can't create body file";
					}

					body << body_text.resultado();
					std::cout << body_text.resultado() << std::endl;
#endif
#if 0
					sys::parser body_text ("../etc/web.plantilla",
										   item->name,
										   *item);
									  

					std::string bname ("generated/" + sys::lower(item->name) + ".html");
					sys::stat_t stat_body (bname);

					std::ofstream body (bname);
					if (! body.is_open()) {
						logp (sys::e_crit, "NO BODY FILE!!! " << bname);
						throw "Can't create body file";
					}

					body << body_text.resultado();
					std::cout << body_text.resultado() << std::endl;
					break;
#endif
				}
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

