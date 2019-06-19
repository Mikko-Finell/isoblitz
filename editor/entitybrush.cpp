#include "entitybrush.hpp"
#include <iostream>

EntityBrush::EntityBrush(Engine & eng) : engine(eng) {
}


void EntityBrush::add_entity(Entity::Type type, const Coordinate & coord) {
    for (auto & e : entities) {
        if (e.get_coordinate() == coord) {
            return;
        }
    }
    auto & entity = entities.emplace_back(engine.entityf.create(engine.wrender, type));
    entity.set_coordinate(coord);
}


void EntityBrush::remove_entity(const Entity::Type & type) {
}
