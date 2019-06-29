#include "database.hpp"
#include "stl.hpp"

Database::Database(const std::string & w) : who(w) {
}

void Database::execute(const char * sqlquery, const callback & step) {
    // TODO easy
    // the database file should not be hardcoded here
    sqlite3 * db;
    sqlite3_stmt * stmt;
    assert(sqlite3_open("../data/testdb.sqlite3", &db) == SQLITE_OK);
    if (sqlite3_prepare(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "SQL Error from " << who << ", sqlite3_prepare: "
                        << sqlite3_errmsg(db) << std::endl;
        std::terminate();
    }
    int code = sqlite3_step(stmt);
    for (; code == SQLITE_ROW; code = sqlite3_step(stmt)) {
        step(stmt);
    }
    if (code != SQLITE_DONE) {
        std::cerr << "SQL Error from " << who << ", sqlite3_step: "
                  << sqlite3_errmsg(db) << std::endl;
        std::terminate();
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
