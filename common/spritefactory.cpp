#include "spritefactory.hpp"
#include <sqlite3.h>
#include <iostream>
#include <cassert>

SpriteFactory::SpriteFactory() {
    const auto sqlquery = R"(
        SELECT sprite.name, entity.name,
            origin_x+x, origin_y+y, w, h, offset_x, offset_y
        FROM entity INNER JOIN sprite
        ON entity.name = sprite.entity
        WHERE sprite.frames IS NULL
    )";
    sqlite3 * db;
    sqlite3_stmt * stmt;

    assert(sqlite3_open("../data/testdb.sqlite3", &db) == SQLITE_OK);
    if (sqlite3_prepare(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "SQL Error from SpriteFactory, sqlite3_prepare: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int result_code = sqlite3_step(stmt);
    while (result_code == SQLITE_ROW) {
        int column = 0;

        std::string sprite_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string entity_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int x = sqlite3_column_int(stmt, column++);
        const int y = sqlite3_column_int(stmt, column++);
        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int ox = sqlite3_column_int(stmt, column++);
        const int oy = sqlite3_column_int(stmt, column++);

        auto & spritemap = entitymap[entity_name];
        auto & spritedata = spritemap[sprite_name];

        spritedata.offset = sf::Vector2i{ox, oy};
        spritedata.spritecoords.left = x;
        spritedata.spritecoords.top = y;
        spritedata.spritecoords.width = w;
        spritedata.spritecoords.height = h;
        spritedata.screencoords.width = w;
        spritedata.screencoords.height = h;

        result_code = sqlite3_step(stmt);
    }
    if(result_code != SQLITE_DONE) {
        std::cerr << "SQL Error: SpriteFactory, sqlite3_step: "
            << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

SpriteData
SpriteFactory::get(const std::string & entity, const std::string & sprite) {
    SpriteData data;
    try {
        data = entitymap.at(entity).at(sprite);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: SpriteManager::get(" << entity << ", " << sprite 
            << ")\n" << std::endl;
        throw;
    }
    return data;
}
