#include "entitybrush.hpp"
#include <iostream>

EntityBrush::EntityBrush(EntityManager & em) : entitym(em) {
}

void EntityBrush::add_entity(Entity::Type type, const Coordinate & coord) {
    if (auto entity = entitym.get_at_coordinate(coord); entity != nullptr) {
        return;
    }

    auto & entity = entitym.create(type);
    entity.set_coordinate(coord);
}

void EntityBrush::remove_entity(const Entity::Type & type) {
}
