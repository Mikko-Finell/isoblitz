#ifndef tilefactory_hpp
#define tilefactory_hpp

#include "tile.hpp"
#include "coordinate.hpp"
#include "util.hpp"
#include "sprite.hpp"
#include <unordered_map>

class TileFactory {
    std::unordered_map<tile_id_t, Tile> tiles;
    RenderSystem & render;

public:
    TileFactory(RenderSystem & rs);
    Tile get(tile_id_t id) const;
    std::vector<Tile> get_all() const;
};

#endif
