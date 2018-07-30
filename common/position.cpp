#include "helper.hpp"
#include "position.hpp"
#include "coordinate.hpp"

Coordinate Position::to_coordinate() const {
    return pixel_to_logic(*this);
}
