#ifndef database_hpp
#define database_hpp

#include <sqlite3.h>
#include <functional>
#include <string>

/** 
 * Database
 * Interface between sqlite and isoblitz.
 *
 * TODO: remove prepare and execute
 */
class Database {
    using callback = std::function<void(sqlite3_stmt *)>;

    sqlite3 * db;
    sqlite3_stmt * stmt;
    const std::string who;

public:
    // param n is the callers name, used for error messaging
    Database(const std::string & n);

    // prepare the sqlquery and the database for usage,
    // calling other methods before this is undefined
    sqlite3_stmt * prepare(const char * sqlquery);

    // execute the callback on every row in query
    void execute(const callback & fn);

    // equivalent to prepase and execute
    void execute(const char * sqlquery, const callback & fn);
};

#endif 
