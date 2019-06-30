#ifndef tilemanager_hpp
#define tilemanager_hpp

#include "tile.hpp"
#include "tilefactory.hpp"
#include "graph.hpp"
#include "stl.hpp"

class TileManager {
    std::list<Tile> tiles;
    TileFactory & factory;

public:
    ~TileManager();
    TileManager(TileFactory & tf);
    Tile & create(Tile::ID id);
    void destroy(Tile & tile);
    void destroy(const Coordinate & coord);
    void clear();

    Tile & get(const Coordinate & coord);
    std::list<Tile *> get(const Coordinate::Region & region);
    std::list<Tile *> get(Tile::ID id);
    Coordinate::Region get_coordinate_bounds() const;
    Position::Region get_pixel_bounds() const;
    Graph generate_graph() const;
    void map(const std::function<void(Tile &)> & fn);

    void serialize(IOWriter & out) const;
    void deserialize(IOReader & in);
};

#endif
