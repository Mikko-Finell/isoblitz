#include "entity.hpp"
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>

Entity::~Entity() {
    clear();
}

Entity::Entity(const std::string & n) 
    : _name(n), animation("DEFAULT")
{
}

void Entity::set_cell(const Cell & c) {
    cell = c;
    auto pos = cell.to_pixel();
    hitbox.set_position(pos);
    animation.sprite.set_position(pos.x, pos.y);
}

void Entity::clear() {
    animation.clear();
}

const std::string & Entity::name() const {
    return _name;
}

std::string Entity::info() const {
    std::stringstream ss; ss << "Entity, " << ", name=" 
        << name() << "\n\t" << cell.info()
        << ", " << cell.to_pixel().info() << std::endl;
    ss << "\t" << hitbox.info() << std::endl;
    return ss.str();
}
