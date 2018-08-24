#include "entityfactory.hpp"
#include "database.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <iostream>

EntityFactory::EntityFactory(AnimationFactory & af, RenderSystem & rs)
    : animf(af), render(rs)
{
    auto step_fn = [&](sqlite3_stmt * stmt){
        int column = 0;

        type_id_t type{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int offset_x = sqlite3_column_int(stmt, column++);
        const int offset_y = sqlite3_column_int(stmt, column++);

        auto pair = entities.emplace(type, Entity{0, type});
        Entity & entity = pair.first->second;
        entity.set_hitbox(Hitbox{offset_x, offset_y, w, h});
    };

    const auto sqlquery = R"(
        SELECT Entity.name, w, h offset_x, offset_y
        FROM Entity INNER JOIN Hitbox
        WHERE Entity.name = Hitbox.entity
    )";
    Database db{"EntityFactory"};
    db.execute(sqlquery, step_fn);
}

Entity EntityFactory::get(const type_id_t & type) const {
    Entity entity;
    try {
        entity = entities.at(type);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: EntityFactory::get("<< type <<")\n" << std::endl;
        throw;
    }

    entity.set_uid(++next_id);
    entity.animation = animf.get(type);
    entity.animation.set_sequence("move-down");
    entity.animation.sprite.set_layer(ENTITY_LAYER);

    return entity;
}

std::vector<type_id_t> EntityFactory::get_all_types() const {
    std::vector<type_id_t> vec;
    vec.reserve(entities.size());
    for (auto & pair : entities) {
        vec.push_back(pair.first);
    }
    return vec;
}
