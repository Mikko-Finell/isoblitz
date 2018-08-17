#ifndef __database_hpp__
#define __database_hpp__

#include <sqlite3.h>
#include <functional>
#include <string>

class Database {
    const std::string who;
    sqlite3 * db;
    using callback_t1 = std::function<void(sqlite3_stmt *, int)>;
    using callback_t2 = std::function<void(sqlite3_stmt *)>;

public:
    Database(const std::string & n);
    void execute(const char * sqlquery, const callback_t1 & fn);
    void execute(const char * sqlquery, const callback_t2 & fn);
};

#endif 
