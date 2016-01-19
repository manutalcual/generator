//
// Clase: parser Copyright (c) 2016 ByTech
// Autor: Manuel Cano Muñoz
// Fecha: Wed Mar 15 16:29:27 2006
// Time-stamp: <2016-01-18 15:41:26 manuel>
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
#include "comandos.hh"

#ifndef parser_hh
#define parser_hh


namespace sys {


	class parser
	{
        class scope_t {
        public:
            sys::conf::block_t::mapblock_t::iterator beg;
            sys::conf::block_t::mapblock_t::iterator end;
            size_t count;
            size_t index;
        };
		typedef std::vector<size_t> vecscopes_t;
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
		bool _ok;
		std::string _str;
		int _i;
		int _size;
		std::string _buf;
		std::string _clase;
		vecscopes_t _scopes;
        vecscope_cols_t _cur_col;
		size_t _count;
		size_t _current_count;

		bool analiza ();
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

		void skipBlanks (int & i);
		void skipBloque (int & i);
		std::string bloque (sys::conf::block_t * blk,
							int & pos);
		std::string captureField (sys::conf::block_t * blk, int & i);
		std::string captureWord (int & i);
		std::string captureOperator (int & i);
		bool resolveOperator (std::string op1, std::string ope, std::string op2);
		bool resolveQuestion (sys::conf::block_t * blk, int & i);
	};

} // end namespace sys


#endif // parser_hh


