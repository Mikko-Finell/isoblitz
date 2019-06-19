#ifndef tilebrush_hpp
#define tilebrush_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"
#include <unordered_set>

class TileBrush {
    Engine & engine;
    std::unordered_set<Coordinate, Coordinate::Hash> tiles;

public:
    TileBrush(Engine & eng);
    void add_tile(Tile::ID id, const Coordinate & coord);
    void remove_tile(const Coordinate & coord);
};

#endif
