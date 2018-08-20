#include "entityfactory.hpp"
#include "database.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <iostream>

namespace {
std::function<void(sqlite3_stmt *)> step_fn;
}

EntityFactory::EntityFactory(AnimationFactory & af, RenderSystem & rs)
    : animf(af), render(rs)
{
    step_fn = [&](sqlite3_stmt * stmt){
        int column = 0;

        std::string entity_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int offset_x = sqlite3_column_int(stmt, column++);
        const int offset_y = sqlite3_column_int(stmt, column++);

        entities.emplace(entity_name, Entity{});
        auto & entity = entities[entity_name];
        entity.hitbox = Hitbox{offset_x, offset_y, w, h};

        //auto & spritemap = entitymap[entity_name];
        //auto & spritedata = spritemap[sprite_name];
    };

    const auto sqlquery = R"(
        SELECT Entity.name, w, h offset_x, offset_y
        FROM Entity INNER JOIN Hitbox
        WHERE Entity.name = Hitbox.entity
    )";
    Database db{"EntityFactory"};
    db.execute(sqlquery, step_fn);
}

Entity EntityFactory::get(const std::string & name) {
    Entity entity;

    try {
        entity = entities.at(name);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: EntityFactory::get("<< name <<")\n" << std::endl;
        throw;
    }

    entity.animation = animf.get(name);
    entity.animation.set_sequence("move-down");
    entity.animation.sprite.set_layer(ENTITY_LAYER);
    return entity;
}
