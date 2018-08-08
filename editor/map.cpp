#include <iostream>
#include <algorithm>
#include <map>
#include <cassert>
#include "common/helper.hpp"
#include "common/timer.hpp"
#include "map.hpp"

void printmap(int w, int h, const std::vector<Tile> & tiles) {
    std::cout << "Printmap: Width=" << w << ", Height=" << h 
              << ", Tile count=" << tiles.size() << std::endl;
    if (w > 40) {
        std::cout << "Map too big to print" << std::endl;
        return;
    }
    std::map<std::pair<int, int>, const Tile*> lookup;
    for (auto & tile : tiles) {
        auto c = tile.coordinate();
        lookup[std::make_pair(c.x, c.y)] = &tile;
    }
    using namespace std;
    cout << " ";
    for (int x = 0; x < w; x++) {
        cout << "--";
    }
    cout << "\n";
    for (int y = 0; y < h; y++) {
        cout << "|";
        for (int x = 0; x < w; x++) {
            try {
                auto tptr = lookup.at(std::make_pair(x, y));
                cout << "[]";
            }
            catch (...) {
                cout << "  ";
            }
        }
        cout << "|\n";
    }
    cout << " ";
    for (int x = 0; x < w; x++) {
        cout << "--";
    }
    std::cout << std::endl;
}

std::pair<int, int> Map::normalize() {
    CASE::ScopeTimer timer{"Map::normalize()"};
    float left = 0, right = 0, top = 0, bottom = 0;
    if (tiles.empty() == false) {
        auto initc = tiles.front().coordinate();
        left = initc.x;
        right = initc.x;
        top = initc.y;
        bottom = initc.y;
    }
    std::size_t tilecount = 0;
    for (auto & tile : tiles) {
        ++tilecount;
        // update bounds
        auto vec2f = tile.coordinate();
        left = std::min(vec2f.x, left);
        right = std::max(vec2f.x, right);
        top = std::min(vec2f.y, top);
        bottom = std::max(vec2f.y, bottom);
    }
    sf::Vector2f offset{-left, -top};
    for (auto & tile : tiles) {
        tile.move(offset);
    }

    std::size_t width = 0;
    std::size_t height = 0;

    if (tilecount) {
        width = std::abs(right - left) + 1;
        height = std::abs(bottom - top) + 1;
    }
    return std::make_pair(width, height);
}

void Map::serialize(std::ostream & out) const {
}

void Map::deserialize(std::istream & in) {
}

Map::Map(gfx::Manager & sm) : spritem(sm) {
}

void Map::undo() {
}

void Map::create(const Tile & newtile) {
    for (auto & tile : tiles) {
        if (tile.coordinate() == newtile.coordinate()) {
            if (tile != newtile) {
                tile = newtile;
                return;
            }
            else {
                return;
            }
        }
    }
    tiles.emplace_back(newtile);
}

void Map::remove(const sf::Vector2f & coord) {
    auto cmp = [coord](const Tile & tile){ return tile.coordinate() == coord; };
    auto itr = std::find_if(tiles.begin(), tiles.end(), cmp);
    auto last = tiles.rbegin();
    if (itr != tiles.end()) {
        *itr = *last;
        tiles.pop_back();
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
}

void Map::on_save(const std::string & s) {
    CASE::ScopeTimer timer{"Map::on_save"};
    if (s != "") {
        name = s;
    }
    const auto [width, height] = normalize();
    const std::size_t tilecount = tiles.size();

    std::ofstream out{filename(), std::ios::binary};
    write(EDITOR_VERSION, out); // TODO: make a system for this
    write(tilecount, out);
    write(width, out);
    write(height, out);

    for (auto & tile : tiles) {
        out << tile;
    }
    out.close();

    std::cout << "Saving " << filename() << std::endl;
    std::cout << "Serialize: Width=" << width << ", Height=" << height
              << ", Tile count=" << tilecount << std::endl;
    printmap(width, height, tiles);
}

void Map::on_load(const std::string & s) {
    CASE::ScopeTimer timer{"Map::on_load"};
    if (s != "") {
        name = s;
    }

    int map_version;
    std::size_t tilecount;
    int width, height;

    std::ifstream in{filename(), std::ios::binary};
    read(map_version, in);
    assert(map_version == EDITOR_VERSION);

    read(tilecount, in);
    read(width, in);
    read(height, in);

    tiles.resize(tilecount, spritem);
    for (auto & tile : tiles) {
        in >> tile;
    }
    in.close();

    std::cout << "Loading " << filename() << std::endl;
    std::cout << "Map size " << width << "x" << height << ", " 
              << tilecount << " tiles.\n";
    printmap(width, height, tiles);

    signal.map_loaded(width, height);
}

void Map::on_setname(const std::string & s) {
    if (s != "") {
        name = s;
    }
    std::cout << "Map name is " << name << std::endl;
}
