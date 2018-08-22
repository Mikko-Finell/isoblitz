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

        auto pair = entities.emplace(type, Entity{type});
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
    Entity entity{type};
    try {
        entity = entities.at(type);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: EntityFactory::get("<< type <<")\n" << std::endl;
        throw;
    }
    entity.init(++next_id, animf);
    return entity;
}

std::vector<Entity> EntityFactory::get_all() const {
    std::vector<Entity> vec;
    vec.reserve(entities.size());
    for (auto & pair : entities) {
        vec.push_back(get(pair.first));
        vec.back().init(0, animf);
    }
    return vec;
}
