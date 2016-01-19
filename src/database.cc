//
// Clase: parser Copyright (c) 2016 ByTech
// Autor: Manuel Cano Muñoz
// Fecha: Wed Mar 15 16:29:27 2006
// Time-stamp: <2016-01-18 13:57:52 manuel>
//
//
// Includes
//
#include "database.hh"


QSqlDatabase database::_db;
bool database::_initialized = false;
size_t database::_users = 0;

database::database ()
    : _ok (false),
      _transaction (false)
{
    if (! _initialized) {
        _db = QSqlDatabase::addDatabase("HULK");
        _db.setHostName("localhost");
        _db.setDatabaseName("./hulk.db");
        _db.setUserName("root");
        _db.setPassword("");
        _ok = db.open();
        _initialized = true;
    } else {
        _ok = _initialized;
    }
    ++_users;
}

database::~database ()
{
    if (--_users == 0 && _initialized) {
        _db.close ();
        _initialized = false;
    }
}

bool database::exec (QSqlQuery & query)
{
    bool ret = false;
    bool res = query.exec();

    _queries.push_back (query.executedQuery());
    ret = !res.isValid();

    if (res.isValid()) {
        _errors.push_back (res);
        if (_transaction)
            rollback ();
    }

    return ret;
}

bool database::transaction ()
{
    _transaction = _db.transaction();
    return _transaction;
}

bool database::commit ()
{
    bool ret = false;

    if (! _db.commit()) {
        _errors.push_back (_db.lastError());
        if (!_db.rollback())
            ret = false;
    } else {
        ret = true;
    }

    _transaction = false;

    return ret;
}

bool database::rollback ()
{
    bool ret = false;

    ret = _db.roolback();
    if (!ret)
        _errors.push_back (_db.lastError());

    _transaction = false;

    return ret;
}

