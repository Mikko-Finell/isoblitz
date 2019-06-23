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

void EntityManager::map(const std::function<void(Entity &)> & fn) {
    for (auto & entity : entities) {
        fn(entity);
    }
}

Entity * EntityManager::get_at_coordinate(const Coordinate & coord) {
    for (auto & entity :entities) {
        if (entity.get_coordinate() == coord) {
            return &entity;
        }
    }
    return nullptr;
}

std::list<Entity *> EntityManager::get_in_region(const Coordinate::Region & region) {
    std::list<Entity *> es;
    for (auto & entity : entities) {
        if (region.contains(entity.get_coordinate())) {
            es.push_back(&entity);
        }
    }
    return es;
}

std::list<Entity *> EntityManager::get_in_region(const Position::Region & region) {
    std::list<Entity *> es;
    for (auto & entity : entities) {
        if (region.intersects(entity.hitbox)) {
            es.push_back(&entity);
        }
    }
    return es;
}

void EntityManager::deserialize(IOReader & in) {
    std::size_t entity_count = 0;
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
