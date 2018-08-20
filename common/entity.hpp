#ifndef entity_hpp
#define entity_hpp

#include "hitbox.hpp"
#include "animation.hpp"

using cell_t = sf::Vector2f;

class Entity {
public:
    cell_t cell;
    Animation animation;
    Hitbox hitbox;

    void set_cell(const cell_t & c);
    void update(time_t dt);
};

#endif
