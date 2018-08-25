#include "entity.hpp"
#include <cassert>
#include <iostream>

Entity::Entity(const uid_t & id, const type_id_t & type) 
    : uid(id), type_id(type), animation(type)
{
}

void Entity::update(time_t dt) {
}

void Entity::set_cell(const cell_t & c) {
    cell = c;
    auto pos = cell.to_pixel();
    hitbox.set_position(pos);
    animation.sprite.set_position(pos.x, pos.y);
}

void Entity::set_hitbox(const Hitbox & hb) {
    hitbox = hb;
}

void Entity::serialize(std::ostream & out) const {
    throw std::logic_error{"Entity::serialize not implemented"};
}

void Entity::deserialize(std::istream & in) {
    throw std::logic_error{"Entity::deserialize not implemented"};
}

// EntitySystem /////////////////////////////////////////////////////////////////

void EntitySystem::remove(GameObject * go) {
    auto entity = dynamic_cast<Entity *>(go);
    assert(entity != nullptr);
    remove(entity);
}

void EntitySystem::remove(Entity * entity) {
    entities.erase(entity);
    entity->reg(nullptr);
}

void EntitySystem::add(Entity * entity) {
    entities.insert(entity);
}

void EntitySystem::update(time_t dt) {
    for (auto & entity : entities) {
        entity->update(dt);
    }
}
