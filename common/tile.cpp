#include "tile.hpp"
#include <iostream>
#include <sstream>

Tile::Tile(Tile::ID i) : id(i) {
}

void Tile::set_coordinate(int x, int y) {
    set_coordinate(Coordinate{x, y});
}

void Tile::set_coordinate(const Coordinate & c) {
    sprite.set_position(c.to_pixel());
    this->coordinate = c;
}

void Tile::set_id(Tile::ID id) {
    this->id = id;
}

sf::Vector2f Tile::get_position() const {
    return coordinate.to_pixel();
}

Coordinate Tile::get_coordinate() const {
    return coordinate;
}

Coordinate::Region Tile::get_region() const {
    return Tile::get_region(coordinate);
}

Coordinate::Region Tile::get_region(const Coordinate & coord) {
    return {coord.x - config::cols_per_tile/2, coord.y - config::rows_per_tile/2,
            config::cols_per_tile, config::rows_per_tile};
}

Tile::ID Tile::get_id() const {
    return id;
}

bool Tile::contains(const Coordinate & c) const {
    // Note: The meaning of coordinate for a tile is the same as visual center.
    // That means if tile has 8x8 coordinates, then the upper-left corner is at
    // (x-4,y-4) while lower-right at (x+4,y+4).
    return c.x >= coordinate.x - config::cols_per_tile/2
       and c.y >= coordinate.y - config::rows_per_tile/2
       and c.x < coordinate.x + config::cols_per_tile/2
       and c.y < coordinate.y + config::rows_per_tile/2;
}

bool Tile::intersects(const Tile & other) const {
    return std::abs(coordinate.x - other.coordinate.x) < config::cols_per_tile
       and std::abs(coordinate.y - other.coordinate.y) < config::rows_per_tile;
}

std::string Tile::info() const {
    std::stringstream ss; ss << "Tile, id = " << get_id() <<
        "\nSprite = " << sprite.info() << "\nCoordinate = " << coordinate.info();
    return ss.str();
}
