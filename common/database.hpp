#ifndef database_hpp
#define database_hpp

#include <sqlite3.h>
#include <functional>
#include <string>

class Database {
    using callback = std::function<void(sqlite3_stmt *)>;

    sqlite3 * db;
    sqlite3_stmt * stmt;
    const std::string who;

public:
    Database(const std::string & n);
    sqlite3_stmt * prepare(const char * sqlquery);
    void execute(const char * sqlquery, const callback & fn);
    void execute(const callback & fn);
};

#endif 
