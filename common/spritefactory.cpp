#include "spritefactory.hpp"
#include "rendersystem.hpp"
#include "database.hpp"
#include "stl.hpp"

SpriteFactory::SpriteFactory(SpriteManager & sm, RenderSystem & rs) : spritem(sm), default_rs(rs) {
    const auto sqlquery = R"(
        SELECT Sprite.name,
            Sprite.x, Sprite.y, Sprite.w, Sprite.h,
            Sprite.offset_x, Sprite.offset_y
        FROM Sprite;
    )";

    Database db{"SpriteFactory"};
    db.execute(sqlquery, [&](sqlite3_stmt * stmt){
        int column = 0;

        std::string sprite_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int x = sqlite3_column_int(stmt, column++);
        const int y = sqlite3_column_int(stmt, column++);
        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int ox = sqlite3_column_int(stmt, column++);
        const int oy = sqlite3_column_int(stmt, column++);

        auto & sprite = sprites[sprite_name];
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

Sprite SpriteFactory::create(RenderSystem & rs, const std::string & sprite_name) {
    SpriteImpl * impl = spritem.alloc();
    try {
        *impl = sprites.at(sprite_name);
    }
    catch (std::out_of_range) {
        spritem.destroy(impl);
        std::cerr << "\nERROR: SpriteManager::create1(" << sprite_name << ")\n" << std::endl;
        throw;
    }
    rs.add(impl, "SpriteFactory::create(" + sprite_name + ")");
    return Sprite{&rs, &spritem, this, impl};
}

Sprite SpriteFactory::create(const std::string & sprite_name)
{
    return create(default_rs, sprite_name);
}

SpriteImpl SpriteFactory::create_impl(const std::string & sprite_name)
{
    try {
        return sprites.at(sprite_name);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: SpriteManager::create2(" << sprite_name << ")\n" << std::endl;
        throw;
    }
}

Sprite SpriteFactory::create_from_impl(RenderSystem & rs, const SpriteImpl * source)
{
    return Sprite{&rs, &spritem, this, this->copy(rs, source)};
}

