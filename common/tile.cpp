#include "tile.hpp"
#include <iostream>

Tile::Tile(Tile::ID i) : id(i) {
}

void Tile::set_coordinate(int x, int y) {
    set_coordinate(Coordinate{x, y});
}

void Tile::set_coordinate(const Coordinate & c) {
    Position pos = c.to_pixel();
    sprite.set_position(pos);
    this->coord = c;
}

void Tile::set_id(Tile::ID id) {
    this->id = id;
}

sf::Vector2f Tile::get_position() const {
    return coord.to_pixel();
}

Coordinate Tile::get_coordinate() const {
    return coord;
}

Tile::ID Tile::get_id() const {
    return id;
}

void Tile::serialize(IOWriter & out) {
    out.write(coord);
    out.write(id);
}

void Tile::deserialize(IOReader & in) {
    in.read(coord);
    in.read(id);
}
