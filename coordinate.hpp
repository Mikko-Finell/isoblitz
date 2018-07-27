#ifndef __COORD__
#define __COORD__

#include "position.hpp"
#include "serializable.hpp"

class Coordinate : public Serializable, public sf::Vector2f {
    void serialize(std::ostream & out) override;
    void deserialize(std::istream & in) override;

public:
    /*
    using sf::Vector2f::Vector2f;
    */
    Coordinate();
    Coordinate(float _x, float _y);
    Coordinate(const Coordinate & other);

    void snap_to_grid();
    Coordinate snapped_to_grid();
    //static Coordinate from_position(const Position & pos);
    Position to_position();

    //virtual ~Coordinate() {}
};

#endif
