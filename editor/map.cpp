#include "map.hpp"
#include "common/util.hpp"
#include <CASE/timer.hpp>
#include <iostream>
#include <algorithm>
#include <map>
#include <cassert>
/*
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
*/
Map::Map(RenderSystem & rs) : render(rs) {
}

void Map::undo() {
}

void Map::create(const Tile & newtile) {
    for (auto & tile : tiles) {
        if (tile.intersects(newtile)) {
            if (tile.coordinate() == newtile.coordinate()) {
                tile = newtile;
            }
            return;
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

    std::string tmp_filename;
    if (s == "") {
        tmp_filename = mapdir + name + extension;
    }
    else {
        tmp_filename = mapdir + s + extension;
    }
    if (std::ofstream out{tmp_filename, std::ios::binary}; out.good()) {
        if (s != "") {
            name = s;
        }
        const auto [width, height] = map::save(out, tiles);
        out.close();

        std::cout << "Saving " << filename() << std::endl;
        std::cout << "Serialize: Width=" << width 
                  << ", Height=" << height
                  << ", Tile count=" << tiles.size() << std::endl;
    }
    else {
        std::cerr << "Unable to save " << tmp_filename << std::endl;
    }
}

void Map::on_load(const std::string & s) {
    CASE::ScopeTimer timer{"Map::on_load"};

    std::string tmp_filename;
    if (s == "") {
        tmp_filename = mapdir + name + extension;
    }
    else {
        tmp_filename = mapdir + s + extension;
    }
    if (std::ifstream in{tmp_filename, std::ios::binary}; in.good()) {
        if (s != "") {
            name = s;
        }
        const auto [width, height] = map::load(in, tiles, render);
        in.close();

        std::cout << "Loading " << filename() << std::endl;
        std::cout << "Map size " << width << "x" << height << ", " 
                  << tiles.size() << " tiles.\n";

        signal.map_loaded(width, height);
    }
    else {
        std::cerr << "Unable to load " << tmp_filename << std::endl;
    }
}

void Map::on_setname(const std::string & s) {
    if (s != "") {
        name = s;
    }
    std::cout << "Map name is " << name << std::endl;
}
