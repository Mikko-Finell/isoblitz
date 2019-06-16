#ifndef tilemanager_hpp
#define tilemanager_hpp

#include "tile.hpp"
#include "spritemanager.hpp"
#include <list>

class TileManager {
    std::list<Tile> tiles;
    SpriteManager & spritem;

public:
    TileManager(SpriteManager & sm);
    Tile * alloc();
    void destroy(Tile * tile);
};

#endif
