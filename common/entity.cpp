#include "entity.hpp"
#include <cassert>
#include <iostream>

Entity::Entity(const type_id_t & type) : type_id(type) {
}

Entity::Entity(std::istream & in, AnimationFactory & animf) {
    deserialize(in, animf);
}

void Entity::init(uid_t uid, AnimationFactory & animf) {
    id = uid;
    animation = animf.get(type_id); // TODO animationfactory uses std::string here
    animation.set_sequence("move-down");
    animation.sprite.set_layer(ENTITY_LAYER);
}

void Entity::update(time_t dt) {
    animation.update(dt);
}

void Entity::destroy() {
    animation.sprite.hide();
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
    util::serialize_std_string(type_id, out);
    util::write(id, out);
    cell.serialize(out);
    animation.serialize(out); // util::write(current_sequence)
    hitbox.serialize(out);
}

void Entity::deserialize(std::istream & in, AnimationFactory & animf) {
    type_id = util::deserialize_std_string(in);
    util::read(id, in);
    cell.deserialize(in);
    
    // animation special deserialization
    const auto sequence = util::deserialize_std_string(in);

    animation = animf.get(type_id);
    animation.set_sequence(sequence);

    hitbox.deserialize(in);
    set_cell(cell);
}
