#include <iostream>
#include <algorithm>
#include <cassert>
#include "map.hpp"

void Map::serialize(std::ostream & out) const {
    std::list<Tile> out_tiles;
    std::for_each(tiles.begin(), tiles.end(),
        [&out_tiles](auto & stack){
            const auto & tile = stack.top();
            if (tile.is_empty_tile() == false) {
                out_tiles.push_back(tile);
            }
        });
    write(out_tiles.size(), out);
    std::for_each(out_tiles.begin(), out_tiles.end(),
        [&out](const Tile & tile){ out << tile; });
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
    auto sort_fn = [](auto & lhs, auto & rhs){
        return lhs.top() < rhs.top();
    };
    tiles.sort(sort_fn);
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
    else if (event == Event::Save) {
        if (auto newname = std::get_if<std::string>(&event.param)) {
            name = *newname;
        }
        std::ofstream out{name + extension, std::ios::binary};
        std::cout << "Saving " << name + extension << std::endl;
        serialize(out);
        out.close();
    }
    else if (event == Event::Load) {
        if (auto newname = std::get_if<std::string>(&event.param)) {
            name = *newname;
        }
        std::ifstream in{name + extension, std::ios::binary};
        std::cout << "Loading " << name + extension << std::endl;
        deserialize(in);
        in.close();
    }
    else if (event == Event::New) {
        if (auto newname = std::get_if<std::string>(&event.param)) {
            name = *newname;
        }
        else {
            name = "tmp";
        }
        tiles.clear();
        history.clear();
    }
    else if (event == Event::SetMapName) {
        name = std::get<std::string>(event.param);
    }
}
