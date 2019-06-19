#ifndef tilecursor_hpp
#define tilecursor_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"

class TileCursor {
    Tile cursor;
    Engine & engine;

public:
    ~TileCursor();
    TileCursor(Engine & engine);
    void set_tile_type(Tile::ID id);
    void update_mousepos(const Position & pos);
    Tile::ID get_id() const;
    Coordinate get_coordinate() const;
};
#endif
