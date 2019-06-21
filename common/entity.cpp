#include "entity.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

Entity::~Entity() {
    signals.im_dead(*this);
    clear();
}

Entity::Entity(const Entity::Type & n) 
    : type(n), animation("DEFAULT")
{
}

void Entity::set_coordinate(const Coordinate & coord) {
    coordinate = coord;
    auto pos = coord.to_pixel();
    hitbox.set_position(pos);
    animation.sprite.set_position(pos);
}

void Entity::clear() {
    animation.clear();
}

Coordinate Entity::get_coordinate() const {
    return coordinate;
}

Position Entity::get_position() const {
    return coordinate.to_pixel();
}

const Entity::Type & Entity::get_type() const {
    return type;
}

bool Entity::operator==(const Entity & other) const {
    return this == &other;
}

void Entity::serialize(IOWriter & out) const {
    out.write(type);
    out.write(coordinate);
    //out.write(animation.current_sequence());
}

std::string Entity::info() const {
    std::stringstream ss; ss << "Entity, " << ", type=" 
        << get_type() << "\n\t" << coordinate.info()
        << ", " << coordinate.to_pixel().info() << std::endl;
    ss << "\t" << hitbox.info() << std::endl;
    return ss.str();
}
