#include "coordinate.hpp"
#include "helper.hpp"

Coordinate::Coordinate() 
{
}

Coordinate::Coordinate(float _x, float _y) 
    : sf::Vector2f{_x, _y}
{
}

Coordinate::Coordinate(const Coordinate & other)
    : sf::Vector2f{other.x, other.y}
{
}

void Coordinate::serialize(std::ostream & out) {
    write(x, out);
    write(y, out);
}

void Coordinate::deserialize(std::istream & in) {
    read(x, in);
    read(y, in);
}

void Coordinate::snap_to_grid() {
    x = floor(x);
    y = floor(y);
}

Coordinate Coordinate::snapped_to_grid() {
    return Coordinate(floor(x), floor(y));
}

/*Coordinate Coordinate::from_position(const Position & pos) {
    return Coordinate{tile_center_at(pos)};
}*/

Position Coordinate::to_position() {
    return logic_to_pixel(*this);
}
