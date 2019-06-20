#ifndef tilemanager_hpp
#define tilemanager_hpp

#include "tile.hpp"
#include "tilefactory.hpp"
#include "spritemanager.hpp"
#include <list>

class TileManager {
    std::list<Tile> tiles;
    TileFactory & factory;

public:
    ~TileManager();
    TileManager(TileFactory & tf);
    Tile & create(Tile::ID id);
    void destroy(Tile & tile);
    void destroy(const Coordinate & coord);
    Tile & get(const Coordinate & coord);
    std::list<Tile *> get(const Coordinate::Region & region);
    std::list<Tile *> get(Tile::ID id);
    void clear();

    void serialize(IOWriter & out) const;
    void deserialize(IOReader & in);
};

#endif
