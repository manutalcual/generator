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
// Program: generator Copyright (c) 2015 By Tech
// Author: Manuel Cano Muñoz
// Date: Mon Dec 14 09:38:54 2015
// Time-stamp: <>
// 
// $Id$
//
//
// Includes
//
#include <string>
#include <fstream>

#include "common.hh"
#include "conf.hh"
#include "stat.hh"


#ifndef generator_hh
#define generator_hh


namespace app {

	class ex : public std::exception
	{
	public:
		ex (const char * msg, const char * file, int line)
			: _file (file),
			  _line (line),
			  _msg (msg)
			{ }
	protected:
		const char * _file;
		int _line;
		const char * _msg;
	};

	class generator
	{
	public:
		class create : public ex
		{
		public:
			create (const char * msg,
					const char * file,
					int line)
				: ex (msg, file, line)
				{ }
		};

		class file : public ex
		{
		public:
			file (const char * msg,
				  const char * file,
				  int line)
				: ex (msg, file, line)
				{ }
		};

		generator (sys::conf & pconf);
		~generator ();
		bool make_project () throw (generator::create);
	protected:
		sys::conf & _conf;
		std::string _path; // Output dir
		std::string _js; // (j)ava(s)cript
		std::string _ch; // (c)urrent class (h)header
		std::string _cb; // (c)urrent class (b)ody
		std::string _po; // (p)rot(o)
		std::string _pd; // (p)roto (d)delete
		std::string _ix; // html (i)nde(x)
		std::string _hl; // (h)tm(l)
		std::string _db; // (d)ata(b)ase
        std::string _conf_path;

	private:

		void create_lib (sys::conf::block_t * item);
		void create_angular (sys::conf::block_t * item);
		void create_html (sys::conf::block_t * item);
		void create_db_tests (sys::conf::block_t * item);
		void create_makefile (sys::conf::block_t * item);
		void create_qtsql (sys::conf::block_t * item);
		void create_qtpro (sys::conf::block_t * item);

		void create_db_main (sys::conf::block_t * item);
		void create_db_details (std::ofstream & out,
								sys::conf::block_t * item);
		void create_db_relations (std::ofstream & out,
								  sys::conf::block_t * item);

		void create_contract_class (sys::conf::block_t * item);
		template<typename Func>
		void create_contract_fields (sys::conf::block_t * item,
									 sys::conf::block_t * fields,
									 Func func);
		template<typename Func>
		void create_contract_field (size_t i, sys::conf::block_t * item, Func func);
		void create_contract_relations (sys::conf::block_t * item);
		void create_contract_relation (sys::conf::block_t * item);

		void create_proto_message (sys::conf::block_t * item);
		void create_proto_fields (sys::conf::block_t * item);
		void create_proto_field (sys::conf::block_t * item, int num);
		void create_proto_relations (sys::conf::block_t * item);
		void create_proto_relation (sys::conf::block_t * item);

		void create_angular_module (sys::conf::block_t * item);
		void create_angular_route (sys::conf::block_t * item);
		void create_angular_controller (sys::conf::block_t * item);
		void create_angular_net (sys::conf::block_t * item);
		void create_angular_data (sys::conf::block_t * item);

		void create_html_main (sys::conf::block_t * item);
		void create_html_edit (sys::conf::block_t * item);

		void create_server (sys::conf::block_t * item);
		void create_configure_ac (sys::conf::block_t * item);
		void create_makefile_am (sys::conf::block_t * item);
		void create_sub_makefile_am (sys::conf::block_t * item);

		void create_proto (sys::conf::block_t * item);
		void create_proto_enums (sys::conf::block_t * item);

		void create_servers (sys::conf::block_t * item);
		void create_indi_server (sys::conf::block_t * item, int entityNum);

	};

} // end namespace app

#endif // generator_hh
