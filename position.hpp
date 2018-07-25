#ifndef __POSITION__
#define __POSITION__

#include <SFML/System/Vector2.hpp>

class Coordinate;

class Position : public sf::Vector2f {
public:
    using sf::Vector2f::Vector2f;

    Coordinate to_coordinate() const;
};

#endif
