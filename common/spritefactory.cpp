#include "spritefactory.hpp"
#include "database.hpp"
#include <iostream>

SpriteFactory::SpriteFactory() {
    const auto sqlquery = R"(
        SELECT Sprite.name, Entity.name,
            x, y,
            sprite_w, sprite_h,
            sprite_offset_x, sprite_offset_y
        FROM Entity INNER JOIN Sprite
        ON Entity.name = Sprite.entity
    )";

    Database db{"SpriteFactory"};
    db.execute(sqlquery, [&](sqlite3_stmt * stmt){
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
    });
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
