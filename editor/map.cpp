#include <iostream>
#include <algorithm>
#include <map>
#include <cassert>
#include "map.hpp"

void Map::serialize(std::ostream & out) const {
    float left = 0, right = 0, top = 0, bottom = 0;
    if (tiles.empty() == false) {
        auto initc = tiles.front().top().coordinate();
        left = initc.x;
        right = initc.x;
        top = initc.y;
        bottom = initc.y;
    }
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
    float width = 0, height = 0;
    std::size_t size = out_tiles.size();
    if (size) {
        width = std::abs(right - left) + 1;
        height = std::abs(bottom - top) + 1;
    }
    // write out
    write(size, out);
    write(width, out);
    write(height, out);
    for (auto & tile : out_tiles) {
        tile.move(offset);
        out << tile;
    }
}

void printmap(float w, float h, std::list<std::stack<Tile>> & tiles) {
    std::map<std::pair<float, float>, Tile*> lookup;
    for (auto & stack : tiles) {
        auto & tile = stack.top();
        auto c = tile.coordinate();
        lookup[std::make_pair(c.x, c.y)] = &tile;
    }
    for (float f = -1; f < w; f += 1) {
        std::cout << "--";
    }
    std::cout << "\n";
    for (float y = 0; y < h; y += 1) {
        std::cout << "|";
        for (float x = 0; x < w; x += 1) {
            Tile * tptr = nullptr;
            try {
                tptr = lookup.at(std::make_pair(x, y));
            }
            catch (...) {
                std::cout << "  ";
                continue;
            }
            std::cout << "[]";
        }
        std::cout << "|\n";
    }
    for (float f = -1; f < w; f += 1) {
        std::cout << "--";
    }
    std::cout << std::endl;
}

void Map::deserialize(std::istream & in) {
    tiles.clear();
    std::size_t size;
    float width, height;
    read(size, in);
    read(width, in);
    read(height, in);
    std::cout << "Map size " << width << "x" << height << ", ";
    std::cout << size << " tiles.\n";
    while(size--) {
        Tile tile;
	in >> tile;
        create(tile);
    }
    printmap(width, height, tiles);
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

void Map::on_new(const std::string & s) {
    if (s != "") {
        name = s;
    }
    else {
        name = "tmp";
    }
    tiles.clear();
    history.clear();
}

void Map::on_save(const std::string & s) {
    if (s != "") {
        name = s;
    }
    std::ofstream out{filename(), std::ios::binary};
    std::cout << "Saving " << filename() << std::endl;
    serialize(out);
    out.close();
}

void Map::on_load(const std::string & s) {
    if (s != "") {
        name = s;
    }
    std::ifstream in{filename(), std::ios::binary};
    std::cout << "Loading " << filename() << std::endl;
    deserialize(in);
    in.close();
}

void Map::on_setname(const std::string & s) {
    if (s != "") {
        name = s;
    }
    std::cout << "Map name is " << name << std::endl;
}
