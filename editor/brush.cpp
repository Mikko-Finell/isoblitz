#include "brush.hpp"
#include <iostream>

Brush::Brush(TileFactory & tf, Map & m) : tilef(tf), map(m) {
    tile = tilef.get(1);
}

void Brush::on_tile_selected(tile_id_t id) {
    tile = tilef.get(id);
}

void Brush::paint() {
    map.add_tile(tile.get_id(), tile.get_coordinate());
}

void Brush::erase() {
    map.remove_tile(tile.get_coordinate());
}

void Brush::set_coordinate(const coord_t & coord) {
    tile.set_coordinate(coord);
}
