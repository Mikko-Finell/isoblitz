#include "map.hpp"
#include <iostream>

Map::Map(RenderSystem & rs, TileFactory & tf) : render(rs), tilef(tf) {
}

// add a tile: if no tile exists at coord, create new tile;
// otherwise copy-assign existing tile with new tile of type id.
void Map::add_tile(const tile_id_t & id, const coord_t & coord) {
    /*
    auto cmp = [coord](const Tile * tile){
        return tile->get_coordinate() == coord;
    };
    auto itr = std::find_if(tiles.begin(), tiles.end(), cmp);
    if (itr == tiles.end()) {
        tiles.push_back(tilef.create(render, id));
        tiles.back()->set_coordinate(coord);
    }
    else {
        Tile * oldtile = *itr;
        if (id != oldtile->get_id()) {
            oldtile = tilef.create(render, id);
            oldtile->set_coordinate(coord);
        }
    }
    */
}

// fast erase by swapping with back() and pop_back()
void Map::remove_tile(const coord_t & coord) {
    /*
    auto cmp = [coord](const Tile & tile){
        return tile->get_coordinate() == coord;
    };
    auto itr = std::find_if(tiles.begin(), tiles.end(), cmp);
    if (itr != tiles.end()) {
        Tile * lasttile = tiles.back();
        Tile * removetile = *itr;
        removetile = lasttile;
        tiles.pop_back();
    }
    */
}

void Map::serialize(std::ostream & out) const {
    /*
    util::write(tiles.size(), out);
    for (auto & tile : tiles) {
        tile.serialize(out);
    }
    */
}

void Map::deserialize(std::istream & in) {
    /*
    tiles.clear();
    std::size_t size;
    util::read(size, in);
    tiles.reserve(size);
    for (auto i = 0; i < size; i++) {
        tiles.emplace_back(in, render);
    }
    */
}
