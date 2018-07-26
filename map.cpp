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

void Map::create(const Tile::Type & type, const Coordinate & coord) {
    Tile tile{coord};
    tile.replace_with(type);

    auto result = std::find(tiles.begin(), tiles.end(), tile);
    if (result == tiles.end()) {
	tiles.push_back(tile);
	history.push_back(&tiles.back());
    }
    else {
	auto & existing_tile = *result;
	if (existing_tile.replace_with(type)) {
	    history.push_back(&existing_tile);
	}
    }
}

void Map::remove(const Coordinate & coord) {
    Tile::Type type{Coordinate{0,0}, false};
    create(type, coord);
}

void Map::draw(std::vector<sf::Vertex> & vertices) {
    tiles.sort();
    for (auto & tile : tiles) {
	tile.draw(vertices);
    }
}

void Map::recvevent(Event event) {
    if (event == Event::CreateTile) {
	auto pair = static_cast<std::pair<Tile::Type, Coordinate>*>(event.data);
	auto type = pair->first;
	auto coord = pair->second;
	create(type, coord);
    }
    else if (event == Event::RemoveTile) {
	remove(event.coordinate);
    }
    else if (event == Event::Undo) {
	undo();
    }
}
