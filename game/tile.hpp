#ifndef tile_hpp
#define tile_hpp

#include "common/mapobject.hpp"

using cell_t = sf::Vector2f;

class Tile : public map::MapObject {

public:
    Tile(RenderSystem & rs);
    bool contains(const cell_t & cell) const;
};

#endif 
