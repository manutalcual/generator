/**
 * Copyright (C) ByTech 2016
 * \file database.hh
 * \author Manuel Cano
 * \date 2016-01-18
 * 
 */
#include <string>
#include <vector>

#include <QString>
#include <QSqlDatabase>


#ifndef database_hh
#define database_hh


class database
{
protected:
    typedef std::map<std::string, std::string> mapstring_t;
    typedef std::vector<QSqlError> vecsqlerror_t;
    typedef std::vector<QString> vecsqlqueries_t;
public:
    database ();
    virtual ~database ();
    virtual bool insert () = 0;
    virtual bool update () = 0;
    virtual bool del () = 0;
protected:
    bool _transaction;
    vecsqlerror_t _errors;
    vecsqlqueries_t _queries;

    bool exec (QSqlQuery & query);
    bool transaction ();
    bool commit ();
    bool rollback ();

private:
    static QSqlDatabase _db;
    static bool _initialized;
    static size_t _users;
};

#endif // database_hh
