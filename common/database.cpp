#include "database.hpp"
#include <stdexcept>
#include <iostream>
#include <cassert>

Database::Database(const std::string & n) : who(n) {
}

sqlite3_stmt * Database::prepare(const char * sqlquery) {
    assert(sqlite3_open("../data/testdb.sqlite3", &db) == SQLITE_OK);
    if (sqlite3_prepare(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        std::string err = "SQL Error from " + who + ", sqlite3_prepare: "
                        + sqlite3_errmsg(db);
        throw std::invalid_argument{err};
    }
    return stmt;
}

void Database::execute(const callback & step) {
    int code = sqlite3_step(stmt);
    for (; code == SQLITE_ROW; code = sqlite3_step(stmt)) {
        step(stmt);
    }
    if (code != SQLITE_DONE) {
        std::cerr << "SQL Error from " << who << ", sqlite3_step: "
                  << sqlite3_errmsg(db) << std::endl;
        std::terminate();
    }
}

void Database::execute(const char * sqlquery, const callback & step) {
    prepare(sqlquery);
    execute(step);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
