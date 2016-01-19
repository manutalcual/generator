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
// Program: db Copyright (c) 2015 By Tech
// Author: Manuel Cano Muñoz
// Date: Thu Dec 10 11:59:22 2015
// Time-stamp: <>
// 
// $Id$
//
//
// Includes
//
#include <QxOrm.h>
#include "Hulk_all_include.gen.h"

#include "common.hh"


int main (int argc, char ** argv)
{
	SET_LOG_LEVEL (sys::e_debug);
	flog ();

    QCoreApplication a(argc, argv);

    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName("./hulk.db");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("");
    qx::QxSqlDatabase::getSingleton()->setTraceSqlRecord(true);
    qx::QxSqlDatabase::getSingleton()->setTraceSqlQuery(true);


    //QSharedPointer<Settings>  users = QSharedPointer<Settings>(new Settings());
    Settings users;
    users.setId(1);
    users.setModelVersion("AAAAAAA");

    users.qxInsert();

    list_of_Settings lst;
    users.qxFetchAll(lst);
    foreach(auto l, lst)
    {
        qDebug() << l.second->getModelVersion();
    }

	logp (sys::e_debug, "Exiting from program.");
	return 0;
}



