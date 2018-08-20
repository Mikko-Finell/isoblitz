#ifndef brush_hpp
#define brush_hpp

#include "common/map.hpp"
#include "common/tilefactory.hpp"
#include "common/observer.hpp"

class Brush : public Observer {
    Tile tile;
    TileFactory & tilef;
    Map & map;

public:
    Brush(TileFactory & tf, Map & m);
    void on_tile_selected(tile_id_t id);
    void paint();
    void erase();
    void set_coordinate(const coord_t & coord);
};

#endif
