#include "tile.hpp"

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

sf::Vector2f Tile::get_position() const {
    return coord.to_pixel();
}

Coordinate Tile::get_coordinate() const {
    return coord;
}

Tile::ID Tile::get_id() const {
    return id;
}

void Tile::serialize(std::ostream & out) const {
    util::write(coord.x, out);
    util::write(coord.y, out);
    util::write(id, out);
    util::write(layer, out);
}

void Tile::deserialize(std::istream & in) {
    util::read(coord.x, in);
    util::read(coord.y, in);
    util::read(id, in);
    util::read(layer, in);
}
