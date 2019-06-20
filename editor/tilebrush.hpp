#ifndef tilebrush_hpp
#define tilebrush_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"
#include <unordered_set>
#include <vector>

class TileBrush {
    TileManager & tilem;
    //std::unordered_set<Coordinate, Coordinate::Hash> tiles;
    //std::vector<Tile *> tiles;

public:
    TileBrush(TileManager & tm);
    void add_tile(Tile::ID id, const Coordinate & coord);
    void remove_tile(const Coordinate & coord);
};

#endif
