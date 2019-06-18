#include "tilebrush.hpp"

TileBrush::TileBrush(Engine & eng) : engine(eng) {
}

void TileBrush::add_tile(Tile::ID id, const Tile::Coord & coord) {
    for (auto tile : tiles) {
        if (tile->get_coordinate() == coord) {
            remove_tile(coord);
            break;
        }
    }
    auto tile = engine.tilef.create(engine.wrender, id);
    tile->set_coordinate(coord);
    tiles.push_back(tile);
}

void TileBrush::remove_tile(const Tile::Coord & coord) {
    auto itr = tiles.begin();
    while (itr != tiles.end()) {
        Tile & tile = **itr;
        if (tile.get_coordinate() == coord) {
            tiles.erase(itr);
            engine.tilem.destroy(&tile);
            return;
        }
        itr++;
    }
}
