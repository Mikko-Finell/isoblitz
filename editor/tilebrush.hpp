#ifndef tilebrush_hpp
#define tilebrush_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"
#include <unordered_set>

class TileBrush {
    Engine & engine;
    std::unordered_set<Tile::Coord, Tile::Coord::Hash> tiles;

public:
    TileBrush(Engine & eng);
    void add_tile(Tile::ID id, const Tile::Coord & coord);
    void remove_tile(const Tile::Coord & coord);
};

#endif
