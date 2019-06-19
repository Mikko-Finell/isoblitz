#include "tilebrush.hpp"
#include <iostream>

TileBrush::TileBrush(Engine & eng) : engine(eng) {
}

void TileBrush::add_tile(Tile::ID id, const Tile::Coord & coord) {
    if (tiles.insert(coord).second == true) {
        Tile & tile = engine.tilef.create(engine.wrender, id);
        tile.set_coordinate(coord);
    }
    else {
        remove_tile(coord);
        add_tile(id, coord);
    }
}

void TileBrush::remove_tile(const Tile::Coord & coord) {
    const auto erased = tiles.erase(coord);
    if (erased == 1) {
        engine.tilem.destroy(coord);
    }
    else {
        assert(erased == 0);
    }
}
