#include "entitymanager.hpp"

EntityManager::EntityManager(EntityFactory & ef) : factory(ef) {
}

Entity & EntityManager::create(const Entity::Type & type) {
    Entity & entity = entities.emplace_back(factory.create(type));
    return entity;
}

void EntityManager::destroy(Entity & entity) {
    for (auto itr = entities.begin(); itr != entities.end(); itr++) {
        Entity & e = *itr;
        if (e == entity) {
            entities.erase(itr);
            return;
        }
    }
}

void EntityManager::clear() {
    entities.clear();
}

Entity * EntityManager::get_at_coordinate(const Coordinate & coord) {
    for (auto & entity :entities) {
        if (entity.get_coordinate() == coord) {
            return &entity;
        }
    }
    return nullptr;
}

void EntityManager::deserialize(IOReader & in) {
    decltype(entities.size()) entity_count = 0;
    in.read(entity_count);
    for (int i = 0; i < entity_count; i++) {
        Entity::Type type;
        Coordinate coord;
        in.read(type);
        in.read(coord);
        Entity & entity = create(type);
        entity.set_coordinate(coord);
    }
}

void EntityManager::serialize(IOWriter & out) const {
    out.write(entities.size());
    for (const Entity & entity : entities) {
        out.write(entity.get_type());
        out.write(entity.get_coordinate());
    }
}
