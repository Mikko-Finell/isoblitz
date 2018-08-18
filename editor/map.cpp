#include "map.hpp"
#include "common/util.hpp"
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
    mapinfo = { .name = "tmp" };
}

void Map::load(const std::string & mapname) {
}
