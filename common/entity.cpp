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
    hitbox.set_position(cell); // TODO change to screen coords
    auto pos = util::to_pixel(cell);
    animation.sprite.set_position(pos.x, pos.y);
}

void Entity::set_hitbox(const Hitbox & hb) {
    hitbox = hb;
}

void Entity::serialize(std::ostream & out) const {
    throw std::logic_error{"Entity::serialize not implemented"};
    /*
    util::serialize_std_string(type_id, out);
    util::write(uid, out);
    cell.serialize(out);
    hitbox.serialize(out);
    */
}

void Entity::deserialize(std::istream & in) {
    throw std::logic_error{"Entity::deserialize not implemented"};
    /*
    type_id = util::deserialize_std_string(in);
    util::read(uid, in);
    cell.deserialize(in);
    hitbox.deserialize(in);
    set_cell(cell);
    */
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
