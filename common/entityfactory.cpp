#include "entityfactory.hpp"
#include "database.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <iostream>

EntityFactory::EntityFactory(AnimationFactory & af) : animf(af)
{
    auto step_fn = [&](sqlite3_stmt * stmt){
        int column = 0;

        std::string name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int offset_x = sqlite3_column_int(stmt, column++);
        const int offset_y = sqlite3_column_int(stmt, column++);

        auto pair = entities.emplace(name, name);
        Entity & entity = pair.first->second;
        entity.hitbox = Hitbox{offset_x, offset_y, w, h};
    };

    const auto sqlquery = R"(
        SELECT Entity.name, Hitbox.w, Hitbox.h, Hitbox.offset_x, Hitbox.offset_y
        FROM Entity INNER JOIN Hitbox
        WHERE Entity.name = Hitbox.entity
    )";
    Database db{"EntityFactory"};
    db.execute(sqlquery, step_fn);
}

Entity EntityFactory::create(RenderSystem & rs, const std::string & name) const {
    Entity entity;
    try {
        entity = entities.at(name);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: EntityFactory::get("<< name <<")\n" << std::endl;
        throw;
    }
    entity.animation = animf.create(rs, name);
    entity.animation.sprite.set_layer(config::entity_layer);
    return entity;
}

std::vector<std::string> EntityFactory::get_all() const {
    std::vector<std::string> vec;
    vec.reserve(entities.size());
    for (auto & pair : entities) {
        vec.push_back(pair.first);
    }
    return vec;
}
