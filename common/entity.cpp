#include "entity.hpp"
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>

Entity::~Entity() {
    clear();
}

Entity::Entity(const Entity::Type & n) 
    : type(n), animation("DEFAULT")
{
}

void Entity::set_cell(const Cell & c) {
    cell = c;
    auto pos = cell.to_pixel();
    hitbox.set_position(pos);
    animation.sprite.set_position(pos.x, pos.y);
}

void Entity::set_coordinate(const Tile::Coord & coord) {
    set_cell({coord.x, coord.y});
}

void Entity::clear() {
    animation.clear();
}

Tile::Coord Entity::get_coordinate() const {
    return Tile::Coord{0, 0};
}

const Entity::Type & Entity::get_type() const {
    return type;
}

std::string Entity::info() const {
    std::stringstream ss; ss << "Entity, " << ", type=" 
        << get_type() << "\n\t" << cell.info()
        << ", " << cell.to_pixel().info() << std::endl;
    ss << "\t" << hitbox.info() << std::endl;
    return ss.str();
}
