#ifndef entity_hpp
#define entity_hpp

#include "tile.hpp"
#include "hitbox.hpp"
#include "common/animation.hpp"

class Entity {
public:
    cell_t cell;
    Animation animation;
    Hitbox hitbox;

    void set_cell(const cell_t & c);
    void update(time_t dt);
};

#endif
