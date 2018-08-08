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

    std::ofstream out{filename(), std::ios::binary};
    const auto  [width, height] = save(out, tiles);
    out.close();

    std::cout << "Saving " << filename() << std::endl;
    std::cout << "Serialize: Width=" << width 
              << ", Height=" << height
              << ", Tile count=" << tiles.size() << std::endl;
    //printmap(width, height, tiles);
}

void Map::on_load(const std::string & s) {
    CASE::ScopeTimer timer{"Map::on_load"};
    if (s != "") {
        name = s;
    }

    std::ifstream in{filename(), std::ios::binary};
    const auto [width, height] = load(in, tiles, spritem);
    in.close();

    std::cout << "Loading " << filename() << std::endl;
    std::cout << "Map size " << width << "x" << height << ", " 
              << tiles.size() << " tiles.\n";
    //printmap(width, height, tiles);

    signal.map_loaded(width, height);
}

void Map::on_setname(const std::string & s) {
    if (s != "") {
        name = s;
    }
    std::cout << "Map name is " << name << std::endl;
}
