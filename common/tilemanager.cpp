#include "tilemanager.hpp"

TileManager::TileManager() {
}

Tile & TileManager::alloc() {
    return tiles.emplace_back();
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

Tile & TileManager::get(const Coordinate & coord) {
    for (auto itr = tiles.begin(); itr != tiles.end(); itr++) {
        Tile & tile = *itr;
        if (tile.get_coordinate() == coord) {
            return tile;
        }
    }
    throw std::out_of_range{"Attempt get non-existant tile."};
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
