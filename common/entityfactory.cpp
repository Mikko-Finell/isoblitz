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

        Entity::Name name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int offset_x = sqlite3_column_int(stmt, column++);
        const int offset_y = sqlite3_column_int(stmt, column++);

        auto pair = entities.emplace(name, Entity{0, name});
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

Entity * EntityFactory::create(RenderSystem & rs, const Entity::Name & name) const {
    Entity * entity = entitym.alloc();
    try {
        *entity = entities.at(name);
    }
    catch (std::out_of_range) {
        entitym.destroy(entity);
        std::cerr << "\nERROR: EntityFactory::get("<< name <<")\n" << std::endl;
        throw;
    }

    // TODO hard critical
    // currently nothing is preventing an entity from being copied such that there
    // exists multiple active entities with same uid, must find a solution to that.
    entity->uid(++next_id);
    entity->animation = animf.create(rs, name);
    entity->animation.sprite.set_layer(config::entity_layer);
    entitys.add(entity, "EntityFactory::create");

    return entity;
}

std::vector<Entity::Name> EntityFactory::get_all_types() const {
    std::vector<Entity::Name> vec;
    vec.reserve(entities.size());
    for (auto & pair : entities) {
        vec.push_back(pair.first);
    }
    return vec;
}
