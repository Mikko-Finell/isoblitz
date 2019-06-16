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
    const std::string who;

public:
    Database(const std::string & who);

    // equivalent to prepare and execute
    void execute(const char * sqlquery, const callback & fn);
};

#endif 
