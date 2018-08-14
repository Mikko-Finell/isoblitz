#ifndef __TILE__
#define __TILE__

#include "common/mapobject.hpp"

using cell_t = sf::Vector2f;

class Tile : public map::MapObject {

public:
    Tile(gfx::SpriteManager & spritem);
    bool contains(const cell_t & cell) const;
};

#endif 
