#include "tilebrush.hpp"
#include <iostream>

TileBrush::TileBrush(TileManager & tm) : tilem(tm) {
}

void TileBrush::add_tile(Tile::ID id, const Coordinate & coord) {
    if (tilem.get(Tile::get_region(coord)).empty() == false) {
        return;
    }
    else {
        Tile & tile = tilem.create(id);
        tile.set_coordinate(coord);
    }
}

void TileBrush::remove_tile(const Coordinate & coord) {
    auto tiles = tilem.get(Tile::get_region(coord));
    for (Tile * tile : tiles) {
        if (tile->contains(coord)) {
            tilem.destroy(*tile);
        }
    }
}
