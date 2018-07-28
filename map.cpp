#include <iostream>
#include <algorithm>
#include <cassert>
#include "map.hpp"

void Map::serialize(std::ostream & out) {
    write(tiles.size(), out);
    for (auto & tilestack : tiles) {
        Tile tile = tilestack.top();
	out << tile;
    }
}

void Map::deserialize(std::istream & in) {
    tiles.clear();
    decltype(tiles.size()) size;
    read(size, in);
    assert(size > 0);
    for (int i = 0; i < size; i++) {
        Tile tile;
	in >> tile;
        create(tile);
    }
}

void Map::undo() {
    if (history.empty()) {
	return;
    }
    auto & tilestack = *history.front();
    history.pop_front();
    assert(tilestack.empty() == false);
    tilestack.pop();
    if (tilestack.empty()) {
	auto itr = std::find(tiles.begin(), tiles.end(), tilestack);
	tiles.erase(itr);
    }
}

void Map::create(const Tile & tile) {
    auto search_fn = [&tile](std::stack<Tile> & st){
        return tile.coordinate() == st.top().coordinate();
    };
    auto itr = std::find_if(tiles.begin(), tiles.end(), search_fn);
    if (itr == tiles.end()) {
	tiles.emplace_back();
        tiles.back().push(tile);
	history.push_front(&tiles.back());
        //std::cout << "Created new tile " << tile.debug() << std::endl;
    }
    else {
	auto & tile_stack = *itr;
        auto & existing_tile = tile_stack.top();
	if (tile != existing_tile) {
            tile_stack.push(tile);
	    history.push_front(&tile_stack);
            //std::cout << "Replaced tile " << tile.debug() << std::endl;
	}
    }
}

void Map::remove(const Coordinate & coord) {
    create(Tile::empty_tile(coord));
}

void Map::draw(std::vector<sf::Vertex> & vertices) {
    //tiles.sort();
    for (auto & stack : tiles) {
        auto & tile = stack.top();
	tile.draw(vertices);
    }
}

void Map::recvevent(const Event & event) {
    if (event == Event::CreateTile) {
        auto tile = std::get<Tile>(event.param);
        create(tile);
    }
    else if (event == Event::RemoveTile) {
	remove(std::get<Coordinate>(event.param));
    }
    else if (event == Event::Undo) {
	undo();
    }
}
