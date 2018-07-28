#ifndef __COORD__
#define __COORD__

#include <sstream>
#include "position.hpp"
#include "serializable.hpp"

class Coordinate : public Serializable, public sf::Vector2f {
    void serialize(std::ostream & out) const override;
    void deserialize(std::istream & in) override;

public:
    Coordinate();
    Coordinate(float _x, float _y);
    Coordinate(const Coordinate & other);

    void snap_to_grid();
    Coordinate snapped_to_grid();
    Position to_position();

    std::string debug() const {
        std::stringstream ss;
        ss << "Coord{"<< x<<", "<<y<<"}";
        return ss.str();
    }
};

#endif
