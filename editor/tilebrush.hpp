#ifndef tilebrush_hpp
#define tilebrush_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"
#include <vector>

class TileBrush {
    Engine & engine;
    std::vector<Tile *> tiles;

public:
    TileBrush(Engine & eng);
    void add_tile(Tile::ID id, const Tile::Coord & coord);
    void remove_tile(const Tile::Coord & coord);
};

#endif
