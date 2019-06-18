#include "tilemanager.hpp"

TileManager::TileManager() {
}

Tile * TileManager::alloc() {
    return &tiles.emplace_back();
}

void TileManager::destroy(Tile * tile) {
    for (auto itr = tiles.begin(); itr != tiles.end(); itr++) {
        if (&(*itr) == tile) {
            tiles.erase(itr);
            return;
        }
    }
    throw std::out_of_range{"Attempt destroy non-existant tile."};
}
