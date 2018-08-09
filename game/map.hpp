#ifndef __MAP__
#define __MAP__

#include "tile.hpp"
#include <vector>
#include <array>

class Map {
    std::vector<Tile> tiles;

public:
    Map(const std::string & mapname, gfx::SpriteManager & spritem);
    Tile * const get_tile(const sf::Vector2f & coord);
    std::vector<Tile*> section(float left, float top, float width,float height);
    std::array<Tile *, DIRECTIONS> neighbors(Tile * center);
    Cell * get_cell(const sf::Vector2f & c);
    std::array<Cell *, DIRECTIONS> neighbors(Cell * center);
};

#endif
