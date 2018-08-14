#ifndef __ENTITY__
#define __ENTITY__

#include "tile.hpp"
#include "common/util.hpp"
#include "common/animation.hpp"

class Entity {
public:
    cell_t cell;
    gfx::Animation animation;
    //gfx::Sprite selected;

    Entity();
    void update(gfx::time_t dt);
    void set_cell(const cell_t & c);
};

#endif
