#include "spritefactory.hpp"
#include "rendersystem.hpp"
#include "database.hpp"
#include <iostream>

SpriteFactory::SpriteFactory(SpriteManager & sm, RenderSystem & rs) : spritem(sm), default_rs(rs) {
    const auto sqlquery = R"(
        SELECT Sprite.name, Entity.type,
            Sprite.x + Entity.tileset_origin_x,
            Sprite.y + Entity.tileset_origin_y,
            sprite_w, sprite_h,
            sprite_offset_x, sprite_offset_y
        FROM Entity INNER JOIN Sprite
        ON Entity.type = Sprite.entity
    )";

    Database db{"SpriteFactory"};
    db.execute(sqlquery, [&](sqlite3_stmt * stmt){
        int column = 0;

        std::string sprite_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string entity_type{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int x = sqlite3_column_int(stmt, column++);
        const int y = sqlite3_column_int(stmt, column++);
        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int ox = sqlite3_column_int(stmt, column++);
        const int oy = sqlite3_column_int(stmt, column++);

        auto & spritemap = sprites[entity_type];
        auto & sprite = spritemap[sprite_name];

        sprite.set_offset(ox, oy)
              .set_size(w, h)
              .set_texcoords({x, y, w, h});
    });
}

SpriteImpl * SpriteFactory::copy(RenderSystem & rs, const SpriteImpl * source)
{
    SpriteImpl * impl = spritem.alloc();
    *impl = *source;
    rs.add(impl, "SpriteFactory::copy");
    return impl;
}

Sprite SpriteFactory::copy(RenderSystem & rs, const Sprite & source)
{
    return Sprite{&rs, &spritem, this, this->copy(rs, source.impl)};
}

Sprite
SpriteFactory::create(RenderSystem & rs, const std::string & entity, const std::string & name)
{
    SpriteImpl * impl = spritem.alloc();
    try {
        *impl = sprites.at(entity).at(name);
    }
    catch (std::out_of_range) {
        spritem.destroy(impl);
        std::cerr << "\nERROR: SpriteManager::create1(" << entity << ", " << name 
                  << ")\n" << std::endl;
        throw;
    }
    rs.add(impl, "SpriteFactory::create(" + entity + ": " + name + ")");
    return Sprite{&rs, &spritem, this, impl};
}

Sprite SpriteFactory::create(const std::string & entity, const std::string & name)
{
    return create(default_rs, entity, name);
}

SpriteImpl
SpriteFactory::create_impl(const std::string & entity, const std::string & name)
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

Sprite SpriteFactory::create_from_impl(RenderSystem & rs, const SpriteImpl * source)
{
    return Sprite{&rs, &spritem, this, this->copy(rs, source)};
}

