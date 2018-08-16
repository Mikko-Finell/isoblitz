#include "tile.hpp"
#include "map.hpp"
#include <iostream>

Tile::Tile(RenderSystem & rs) : MapObject{rs} {
}

bool Tile::contains(const cell_t & cell) const {
    auto coord = coordinate();
    sf::FloatRect box{coord.x, coord.y, COLS_PER_TILE, ROWS_PER_TILE};
    return box.contains(cell);
}
