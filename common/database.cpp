#include "database.hpp"
#include <stdexcept>
#include <iostream>
#include <cassert>

Database::Database(const std::string & n) : who(n) {
}

void Database::execute(const char * sqlquery, const callback_t1 & step) {
    sqlite3 * db;
    sqlite3_stmt * stmt;
    assert(sqlite3_open("../data/testdb.sqlite3", &db) == SQLITE_OK);
    if (sqlite3_prepare(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        std::string err = "SQL Error from " + who + ", sqlite3_prepare: "
                        + sqlite3_errmsg(db);
        throw std::invalid_argument{err};
    }
    int code = sqlite3_step(stmt);
    for (; code == SQLITE_ROW; code = sqlite3_step(stmt)) {
        int columns = sqlite3_column_count(stmt);
        step(stmt, columns);
    }
    if (code != SQLITE_DONE) {
        std::cerr << "SQL Error from " << who << ", sqlite3_step: "
                  << sqlite3_errmsg(db) << std::endl;
        std::terminate();
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Database::execute(const char * sqlquery, const callback_t2 & step) {
    auto fn = [&step](sqlite3_stmt * stmt, int columns){ step(stmt); };
    execute(sqlquery, fn);
}
