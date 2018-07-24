#include <iostream>
#include <algorithm>
#include <cassert>
#include "map.hpp"

void Map::undo() {
    if (history.empty()) {
	return;
    }
    auto tile_ptr = history.back();
    tile_ptr->history.pop_back();
    history.pop_back();
    if (tile_ptr->history.empty()) {
	auto itr = std::find(tiles.begin(), tiles.end(), *tile_ptr);
	assert(itr != tiles.end());
	tiles.erase(itr);
    }
}

void Map::create_at(const Coordinate & coord) {
    Tile new_tile{coord};
    Tile::Type type{Coordinate{0,0}, false};
    new_tile.replace_with(type);
    auto result = std::find(tiles.begin(), tiles.end(), new_tile);
    if (result == tiles.end()) {
	tiles.push_back(new_tile);
	history.push_back(&tiles.back());
    }
    else {
	auto & existing_tile = *result;
	if (existing_tile.replace_with(type)) {
	    history.push_back(&existing_tile);
	}
    }
}

void Map::create_at(const Position & pos) {
    auto v = tile_center_at(pos);
    create_at(v);
}

void Map::remove_at(const Coordinate & coord) {
    auto itr = std::find(tiles.begin(), tiles.end(), Tile{coord});
    if (itr != tiles.end()) {
	tiles.erase(itr);
    }
}

void Map::remove_at(const Position & pos) {
    auto v = tile_center_at(pos);
    remove_at(v);
}

void Map::draw(std::vector<sf::Vertex> & vertices) {
    tiles.sort();
    for (auto & tile : tiles) {
	tile.draw(vertices);
    }
}
