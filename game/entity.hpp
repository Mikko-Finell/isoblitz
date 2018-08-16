#ifndef __ENTITY__
#define __ENTITY__

#include "tile.hpp"
#include "common/util.hpp"
#include "common/animation.hpp"

class Entity {
public:
    cell_t cell;
    Animation animation;

    Entity();
    void update(time_t dt);
    void set_cell(const cell_t & c);
};

#endif
