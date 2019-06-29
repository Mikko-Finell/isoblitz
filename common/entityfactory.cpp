#include "entityfactory.hpp"
#include "database.hpp"
#include "util.hpp"
#include "stl.hpp"

EntityFactory::EntityFactory(AnimationFactory & af, RenderSystem & rs)
    : animf(af), default_rs(rs)
{
    auto step_fn = [&](sqlite3_stmt * stmt){
        int column = 0;

        std::string type{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int offset_x = sqlite3_column_int(stmt, column++);
        const int offset_y = sqlite3_column_int(stmt, column++);

        auto pair = entities.emplace(type, type);
        Entity & entity = pair.first->second;
        entity.hitbox = Hitbox{offset_x, offset_y, w, h};
    };

    const auto sqlquery = R"(
        SELECT Entity.type, Hitbox.w, Hitbox.h, Hitbox.offset_x, Hitbox.offset_y
        FROM Entity INNER JOIN Hitbox
        WHERE Entity.type = Hitbox.entity
    )";
    Database db{"EntityFactory"};
    db.execute(sqlquery, step_fn);
}

Entity EntityFactory::create(RenderSystem & rs, const Entity::Type & type) const {
    Entity entity;
    try {
        entity = entities.at(type);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: EntityFactory::get("<< type <<")\n" << std::endl;
        throw;
    }
    entity.animation = animf.create(rs, type);
    entity.animation.sprite.set_layer(config::entity_layer);
    return entity;
}

Entity EntityFactory::create(const std::string & type) const {
    return create(default_rs, type);
}

std::vector<std::string> EntityFactory::get_all() const {
    std::vector<std::string> vec;
    vec.reserve(entities.size());
    for (auto & pair : entities) {
        vec.push_back(pair.first);
    }
    return vec;
}
/*
void EntityFactory::deserialize(IOReader & in) {
    std::size_t entity_count = 0;
    in.read(entity_count);

    Entity::Type type{"DEFAULT"};
    Coordinate coord;
    for (int i = 0; i < entity_count; i++) {
        in.read(type);
        Entity entity = create(default_rs, type);
        in.read(coord);
        entity.set_coordinate(coord);
    }
}
*/
