#include "spritefactory.hpp"
#include "database.hpp"
#include <iostream>

SpriteFactory::SpriteFactory(SpriteManager & sm) : spritem(sm) {
    const auto sqlquery = R"(
        SELECT Sprite.name, Entity.name,
            Sprite.x + Entity.tileset_origin_x,
            Sprite.y + Entity.tileset_origin_y,
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

        auto & spritemap = sprites[entity_name];
        auto & sprite = spritemap[sprite_name];

        sprite.set_offset(ox, oy);
        sprite.set_spritecoords({x, y, w, h});
        sprite.set_size(w, h);
    });
}

Sprite *
SpriteFactory::create(RenderSystem & rs, const std::string & entity, const std::string & name) const 
{
    Sprite * sprite = spritem.alloc();
    try {
        *sprite = sprites.at(entity).at(name);
    }
    catch (std::out_of_range) {
        spritem.destroy(*sprite);
        std::cerr << "\nERROR: SpriteManager::create1(" << entity << ", " << name 
                  << ")\n" << std::endl;
        throw;
    }
    rs.add(sprite, "SpriteFactory::create(" + entity + ": " + name + ")");
    return sprite;
}

Sprite
SpriteFactory::create(const std::string & entity, const std::string & name) const 
{
    try {
        return sprites.at(entity).at(name);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: SpriteManager::create2(" << entity << ", " << name 
                  << ")\n" << std::endl;
        throw;
    }
}

Sprite * SpriteFactory::copy(RenderSystem & rs, const Sprite & source) const
{
    Sprite * sprite = spritem.alloc();
    *sprite = source;
    rs.add(sprite, "SpriteFactory::copy");
    return sprite;
}
