//
// Clase: parser Copyright (c) 2016 ByTech
// Autor: Manuel Cano Muñoz
// Fecha: Wed Mar 15 16:29:27 2006
// Time-stamp: <2016-01-22 00:05:41 manuel>
//
// Includes
//
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "common.hh"
#include "conf.hh"
#include "map_file.hh"
#include "comandos.hh"

#ifndef parser_hh
#define parser_hh


namespace sys {

	typedef conf::block_t tblock_t;
	typedef conf::block_t::mapblock_t::iterator tblockite_t;

	class parser
	{
        class scope_t {
        public:
			tblock_t * block;
			tblockite_t begin;
			tblockite_t end;
            size_t count;
            size_t index;

			scope_t ()
				: block (0),
				  count (0),
				  index (0)
				{}
        };
		typedef std::vector<scope_t> vecscopes_t;
		typedef std::vector<size_t> vecscope_cols_t;
	public:
		parser (std::string fname,
				std::string clase,
				sys::conf::block_t & bl);
		~parser ();
		operator bool ();
		std::string & resultado ();
	protected:
	private:
		sys::conf::block_t & _bloque;
		sys::map_file _file;
		int _size;
		std::string _str;
		bool _ok;
		int _i;
		std::string _clase;
		std::string _buf;
		size_t _cur_scope;
		vecscopes_t _scopes;
        size_t _index;

		bool analiza ();
		std::string escapa (int & i);
		std::string escapa (int & i, char ch);
		std::string escapa_field (int & i);
		std::string escape_slash (int & i);
		std::string process_command (std::string command, int & i);
		std::string process_if (int & i);
		std::string process_for (int & i);
		std::string process_do (int & i);
		std::string capture_word (int & i);
		std::string escape_var (int & i);
		std::string capture_until (int & i, char ch);
		std::string capture_oper (int & i);
		std::string capture_num (int & i);
		

		void skip_blanks (int & i);
		void skip_bloque (int & i);

		/*
		vecscopes_t _scopes;
        vecscope_cols_t _cur_col;
		size_t _count;
		size_t _current_count;

		std::string escapa (int & i);
		std::string escapa (sys::conf::block_t * blk, int & i);
		std::string escapaComando (sys::conf::block_t * blk, int & i);
		std::string skipComando (int & i);
		std::string parseComando (int & i);

		std::string comandoFor (sys::conf::block_t * blk, 
								std::string nom, int & i);
		std::string comandoDo (sys::conf::block_t * blk, 
							   sys::IComando * com, int & pos);
		std::string comandoWhile (sys::conf::block_t * blk, 
								  std::string nom, int & i);

		void skipBloque (int & i);
		std::string bloque (sys::conf::block_t * blk,
							int & pos);
		std::string captureField (sys::conf::block_t * blk, int & i);
		std::string captureWord (int & i);
		std::string captureOperator (int & i);
		bool resolveOperator (std::string op1, std::string ope, std::string op2);
		bool resolveQuestion (sys::conf::block_t * blk, int & i);
		*/
	};

} // end namespace sys


#endif // parser_hh


