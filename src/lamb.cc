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
// Program: lamb Copyright (c) 2016 By Tech
// Author: Manuel Cano Muñoz
// Date: Fri Feb  5 14:08:34 2016
// Time-stamp: <>
// 
// $Id$
//
// Includes
//
#include "lamb.hh"


lamb::lamb ()
	: _the_number (99)
{
}

lamb::lamb (const lamb & obj)
	: _the_number (obj._the_number + 99)
{
}

lamb::~lamb ()
{
}

int lamb::the_number ()
{
	return _the_number;
}

template<typename F>
void lamb::process (F func)
{
	func (*this);
}

enum e_cosas {
	una,
	dos,
	tres
};

template<enum e_cosas I>
bool function (enum e_cosas p)
{
	return I == p;
}

int main (int argc, char ** argv)
{
	lamb object;
	volatile int i = 3; // volatile so no optimization on this

	object.process([&](lamb obj){
			std::cout << "Hey!" << std::endl;
			std::cout << "The captured:  " << object.the_number() << std::endl;
			std::cout << "The parameter: " << obj.the_number() << std::endl;
			std::cout << "The parameter has been copied, and thus, incremented by 99." << std::endl;
			std::cout << "The captured has been referenced, so it has the same value." << std::endl;
		});

	std::cout << "Enum: " << function<una>(una) << "." << std::endl;
	std::cout << "Enum: " << function<dos>(una) << "." << std::endl;
	std::cout << "Enum: " << function<tres>(static_cast<enum e_cosas>(i)) << "." << std::endl;

	return 0;
}
