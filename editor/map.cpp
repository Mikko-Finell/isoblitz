#include <iostream>
#include <algorithm>
#include <map>
#include <cassert>
#include "map.hpp"

void Map::serialize(std::ostream & out) const {
    if (tiles.empty()) {
        return;
    }

    float left, right, top, bottom;
    auto initc = tiles.front().top().coordinate();
    left = initc.x;
    right = initc.x;
    top = initc.y;
    bottom = initc.y;

    std::list<Tile> out_tiles;
    for (auto & stack : tiles) {
        const Tile & tile = stack.top();
        if (tile.is_empty_tile() == false) {
            out_tiles.push_back(tile);
            // update bounds
            auto coord = tile.coordinate();
            left = std::min(coord.x, left);
            right = std::max(coord.x, right);
            top = std::min(coord.y, top);
            bottom = std::max(coord.y, bottom);
        }
    }
    Coordinate offset{-left, -top};

    assert(out_tiles.empty() == false);
    float width = std::abs(right - left);
    float height = std::abs(bottom - top);

    std::size_t size = out_tiles.size();
    write(size, out);
    write(width, out);
    write(height, out);

    for (auto & tile : out_tiles) {
        tile.move(offset);
        out << tile;
    }
}

std::string printmap(float w, float h, std::list<std::stack<Tile>> & tiles) {
    std::stringstream ss;
    std::map<std::pair<float, float>, Tile*> lookup;
    for (auto & stack : tiles) {
        auto & tile = stack.top();
        auto c = tile.coordinate();
        lookup[std::make_pair(c.x, c.y)] = &tile;
    }
    for (float f = -1; f < w + 1; f += 1) {
        ss << "--";
    }
    ss << "\n";
    for (float y = 0; y <= h; y += 1) {
        ss << "|";
        for (float x = 0; x <= w; x += 1) {
            Tile * tptr = nullptr;
            try {
                tptr = lookup.at(std::make_pair(x, y));
            }
            catch (...) {
                ss << "  ";
                continue;
            }
            ss << "[]";
        }
        ss << "|\n";
    }
    for (float f = -1; f < w + 1; f += 1) {
        ss << "--";
    }
    return ss.str();
}

void Map::deserialize(std::istream & in) {
    tiles.clear();
    std::size_t size;
    float width, height;
    read(size, in);
    read(width, in);
    read(height, in);
    for (int i = 0; i < size; i++) {
        Tile tile;
	in >> tile;
        create(tile);
    }
    std::cout << "Map size " << width << "x" << height << ", ";
    std::cout << size << " tiles.\n";
    std::cout << printmap(width, height, tiles) << std::endl;
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
    }
    else {
	auto & tile_stack = *itr;
        auto & existing_tile = tile_stack.top();
	if (tile != existing_tile) {
            tile_stack.push(tile);
	    history.push_front(&tile_stack);
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
        std::ofstream out{filename(), std::ios::binary};
        std::cout << "Saving " << filename() << std::endl;
        serialize(out);
        out.close();
    }
    else if (event == Event::Load) {
        if (auto newname = std::get_if<std::string>(&event.param)) {
            name = *newname;
        }
        std::ifstream in{filename(), std::ios::binary};
        std::cout << "Loading " << filename() << std::endl;
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
