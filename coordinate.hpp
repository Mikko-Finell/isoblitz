#ifndef __COORD__
#define __COORD__

#include "position.hpp"

class Coordinate : public sf::Vector2f {
public:
    using sf::Vector2f::Vector2f;

    void snap_to_grid();
    Coordinate snapped_to_grid();
    //static Coordinate from_position(const Position & pos);
    Position to_position();
};

#endif
