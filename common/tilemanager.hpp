#ifndef tilemanager_hpp
#define tilemanager_hpp

#include "tile.hpp"
#include "spritemanager.hpp"
#include <list>

class TileManager {
    std::list<Tile> tiles;

public:
    TileManager();
    Tile & alloc();
    void destroy(Tile & tile);
    void destroy(const Tile::Coord & coord);
    Tile & get(const Tile::Coord & coord);
    std::list<Tile *> get(Tile::ID id);
};

#endif
