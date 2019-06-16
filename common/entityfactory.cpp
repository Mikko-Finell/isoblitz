#include "entityfactory.hpp"
#include "database.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <iostream>

EntityFactory::EntityFactory(AnimationFactory & af, EntityManager & em, EntitySystem & es)
    : animf(af), entitym(em), entitys(es)
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
        SELECT Entity.name, Hitbox.w, Hitbox.h, Hitbox.offset_x, Hitbox.offset_y
        FROM Entity INNER JOIN Hitbox
        WHERE Entity.name = Hitbox.entity
    )";
    Database db{"EntityFactory"};
    db.execute(sqlquery, step_fn);
}

Entity * EntityFactory::create(RenderSystem & rs, const type_id_t & type) const {
    Entity * entity = entitym.alloc();
    try {
        *entity = entities.at(type);
    }
    catch (std::out_of_range) {
        entitym.destroy(entity);
        std::cerr << "\nERROR: EntityFactory::get("<< type <<")\n" << std::endl;
        throw;
    }

    // TODO hard critical
    // currently nothing is preventing an entity from being copied such that there
    // exists multiple active entities with same uid, must find a solution to that.
    entity->uid(++next_id);
    entity->animation = animf.create(rs, type);
    //entity->animation->set_sequence("idle-down", "EntityFactory::get");
    entity->animation->sprite->set_layer(ENTITY_LAYER);
    entitys.add(entity);

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
