#include "coordinate.hpp"
#include "helper.hpp"

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
