#include "tilemanager.hpp"

TileManager::~TileManager() {
    clear();
}

TileManager::TileManager(TileFactory & tf) : factory(tf) {
}

Tile & TileManager::create(Tile::ID id) {
    return tiles.emplace_back(factory.create(id));
}

void TileManager::destroy(Tile & tile) {
    for (auto itr = tiles.begin(); itr != tiles.end(); itr++) {
        if (&(*itr) == &tile) {
            tiles.erase(itr);
            return;
        }
    }
    throw std::out_of_range{"Attempt destroy non-existant tile."};
}

void TileManager::destroy(const Coordinate & coord) {
    for (auto itr = tiles.begin(); itr != tiles.end(); itr++) {
        Tile & tile = *itr;
        if (tile.get_coordinate() == coord) {
            tiles.erase(itr);
            return;
        }
    }
    throw std::out_of_range{"Attempt destroy non-existant tile."};
}

void TileManager::clear() {
    tiles.clear();
}

Tile & TileManager::get(const Coordinate & coord) {
    for (auto itr = tiles.begin(); itr != tiles.end(); itr++) {
        Tile & tile = *itr;
        if (tile.get_coordinate() == coord) {
            return tile;
        }
    }
    throw std::out_of_range{"Attempt get non-existant tile."};
}

std::list<Tile *> TileManager::get(const Coordinate::Region & region) {
    std::list<Tile *> ts;
    for (auto & tile : tiles) {
        if (region.intersects(tile.get_region())) {
            ts.push_back(&tile);
        }
    }
    return ts;
}

std::list<Tile *> TileManager::get(Tile::ID id) {
    std::list<Tile *> ts;
    for (auto itr = tiles.begin(); itr != tiles.end(); itr++) {
        Tile & tile = *itr;
        if (tile.get_id() == id) {
            ts.push_back(&tile);
        }
    }
    return ts;
}

Graph TileManager::generate_graph() const {
    return Graph{tiles};
}

void TileManager::serialize(IOWriter & out) const {
    out.write(tiles.size());
    for (auto & tile : tiles) {
        out.write(tile.get_id());
        out.write(tile.get_coordinate());
    }
}

void TileManager::deserialize(IOReader & in) {
    Tile::ID id;
    Coordinate coord;
    decltype(tiles.size()) tile_count = 0;
    in.read(tile_count);
    for (int i = 0; i < tile_count; i++) {
        in.read(id);
        in.read(coord);
        Tile & tile = create(id);
        tile.set_coordinate(coord);
    }
}
